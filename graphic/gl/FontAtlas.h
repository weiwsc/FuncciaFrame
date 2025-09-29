// FontAtlas.h - FreeType + HarfBuzz, on-demand glyph atlas (RED, premultiplied-friendly)
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <cstdint>
#include <algorithm>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

// FreeType / HarfBuzz
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <hb.h>
#include <hb-ft.h>

// OpenGL
#include <glad/glad.h>

namespace Funccia::Graphic::GL {

struct ShapedGlyph {
    uint32_t glyphIndex;   // hb glyph id
    float    x;            // pen position in pixels (top-left origin baseline-aware in your renderer)
    float    y;
    float    advanceX;     // pixels
    float    advanceY;     // pixels (usually 0 for horizontal)
    // Atlas rect (filled after ensureGlyph())
    glm::vec2 uv0{0};      // normalized
    glm::vec2 uv1{0};
    glm::vec2 sizePx{0};   // bitmap size in px
    glm::vec2 bearingPx{0}; // (left, top)
};

class FontAtlas {
public:
    struct Glyph {
        glm::vec2 uv0;          // normalized
        glm::vec2 uv1;          // normalized
        glm::ivec2 size;        // bitmap width,height in px
        glm::ivec2 bearing;     // (left, top) in px
        int advance;            // 26.6 fixed (FT units) — but we also give pixel advances via shaping
        uint32_t glyphIndex;    // hb glyph id (FT glyph index)
    };

    FontAtlas(const std::string& fontPath, int pixelHeight,
              int initialAtlasSize = 1024, int pad = 1)
        : m_fontPath(fontPath),
          m_pixelHeight(pixelHeight),
          m_pad(std::max(0, pad)),
          m_atlasSize(std::max(64, initialAtlasSize)) {
        initFT();
        initHB();
        initGL();
    }

    ~FontAtlas() {
        if (m_texture) glDeleteTextures(1, &m_texture);
        if (m_hbFont)  hb_font_destroy(m_hbFont);
        if (m_hbFace)  hb_face_destroy(m_hbFace);
        if (m_face)    FT_Done_Face(m_face);
        if (m_ft)      FT_Done_FreeType(m_ft);
    }

    // No copy
    FontAtlas(const FontAtlas&) = delete;
    FontAtlas& operator=(const FontAtlas&) = delete;

    // Shaping (UTF-8 input). Populates out with glyph indices and advances (in pixels).
    // direction/script/lang optional; sensible defaults for LTR Latin.
    bool shapeUTF8(const std::string& utf8,
                   std::vector<ShapedGlyph>& out,
                   hb_direction_t dir = HB_DIRECTION_LTR,
                   hb_script_t script = HB_SCRIPT_LATIN,
                   const char* lang = "en") const
    {
        out.clear();
        if (!m_hbFont) return false;

        hb_buffer_t* buf = hb_buffer_create();
        hb_buffer_set_direction(buf, dir);
        hb_buffer_set_script(buf, script);
        hb_buffer_set_language(buf, hb_language_from_string(lang, -1));
        hb_buffer_add_utf8(buf, utf8.c_str(), (int)utf8.size(), 0, (int)utf8.size());

        hb_shape(m_hbFont, buf, nullptr, 0);

        unsigned int count = 0;
        hb_glyph_info_t const* infos  = hb_buffer_get_glyph_infos(buf, &count);
        hb_glyph_position_t const* pos = hb_buffer_get_glyph_positions(buf, &count);

        out.reserve(count);
        float penX = 0.0f, penY = 0.0f;

        // HarfBuzz advances/offsets are in 26.6 (font units scaled to pixel height via hb-ft)
        const float scale = 1.0f / 64.0f;

        for (unsigned int i = 0; i < count; ++i) {
            ShapedGlyph sg{};
            sg.glyphIndex = infos[i].codepoint;
            sg.x = penX + pos[i].x_offset * scale;
            sg.y = penY - pos[i].y_offset * scale; // y grows down in our UI space
            sg.advanceX = pos[i].x_advance * scale;
            sg.advanceY = -pos[i].y_advance * scale;

            // We don't fill UV/size/bearing here; you can call ensureGlyph() when batching.
            out.push_back(sg);

            penX += sg.advanceX;
            penY += sg.advanceY;
        }

        hb_buffer_destroy(buf);
        return true;
    }

    // Ensure glyph is baked into atlas; returns pointer to Glyph info (cached).
    // Returns nullptr if rasterization fails.
    const Glyph* ensureGlyph(uint32_t glyphIndex) {
        auto it = m_glyphs.find(glyphIndex);
        if (it != m_glyphs.end()) return &it->second;

        // Load and render glyph with FreeType
        if (FT_Load_Glyph(m_face, glyphIndex, FT_LOAD_RENDER)) {
            return nullptr;
        }
        FT_GlyphSlot slot = m_face->glyph;
        FT_Bitmap& bm    = slot->bitmap;

        // Expand atlas if needed
        if (!findShelfRect((int)bm.width, (int)bm.rows, m_pad, m_placeX, m_placeY, m_rowH)) {
            if (!growAtlas()) return nullptr;
            // after grow, retry
            if (!findShelfRect((int)bm.width, (int)bm.rows, m_pad, m_placeX, m_placeY, m_rowH)) {
                return nullptr;
            }
        }

        // Copy into CPU atlas (expand storage on first use or after grow)
        if (m_cpuAtlas.empty()) m_cpuAtlas.assign((size_t)m_atlasSize * m_atlasSize, 0);

        const int dstX = m_placeX + m_pad;
        const int dstY = m_placeY + m_pad;
        blitBitmapToAtlas(bm, dstX, dstY);

        // Upload subimage to GPU
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        dstX, dstY,
                        (GLsizei)bm.width, (GLsizei)bm.rows,
                        GL_RED, GL_UNSIGNED_BYTE, bm.buffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Record glyph UVs and metrics
        Glyph g{};
        g.glyphIndex = glyphIndex;
        g.size    = { (int)bm.width, (int)bm.rows };
        g.bearing = { slot->bitmap_left, slot->bitmap_top };
        g.advance = (int)slot->advance.x; // 26.6 fixed

        float u0 = (dstX) / float(m_atlasSize);
        float v0 = (dstY) / float(m_atlasSize);
        float u1 = (dstX + (int)bm.width) / float(m_atlasSize);
        float v1 = (dstY + (int)bm.rows)  / float(m_atlasSize);
        g.uv0 = { u0, v0 };
        g.uv1 = { u1, v1 };

        auto [insIt, _] = m_glyphs.emplace(glyphIndex, g);
        return &insIt->second;
    }

    // Convenience: ensure ASCII glyph by codepoint (not shaped).
    const Glyph* glyphForChar(unsigned char c) { return ensureGlyph(charToGlyphIndex(c)); }

    // Fill atlas-related fields for already-shaped glyphs (UV, size, bearing)
    // You can call this right before you generate your draw vertices.
    void populateAtlasData(std::vector<ShapedGlyph>& shaped) {
        for (auto& sg : shaped) {
            if (const Glyph* g = ensureGlyph(sg.glyphIndex)) {
                sg.uv0 = g->uv0;
                sg.uv1 = g->uv1;
                sg.sizePx    = glm::vec2(g->size);
                sg.bearingPx = glm::vec2(g->bearing);
            }
        }
    }

    // Metrics (pixels at scale 1)
    float ascent()  const { return m_ascent;  }
    float descent() const { return m_descent; } // positive magnitude
    float lineGap() const { return m_lineGap; }

    // GL atlas
    GLuint texture()   const { return m_texture; }
    int    atlasSize() const { return m_atlasSize; }
    int    pad()       const { return m_pad; }

    // Map Unicode codepoint to FT glyph index via HarfBuzz face
    uint32_t charToGlyphIndex(uint32_t codepoint) const {
        hb_codepoint_t glyph_id;
        if (hb_font_get_glyph(m_hbFont, codepoint, 0, &glyph_id)) {
            return glyph_id;
        }
        return FT_Get_Char_Index(m_face, codepoint);
    }

    // ---- State ----
    std::string m_fontPath;
    int m_pixelHeight = 48;
    int m_pad = 1;

    // FreeType / HarfBuzz
    FT_Library m_ft = nullptr;
    FT_Face    m_face = nullptr;
    hb_face_t* m_hbFace = nullptr;
    hb_font_t* m_hbFont = nullptr;

    // Metrics (pixels)
    float m_ascent  = 0.f;
    float m_descent = 0.f; // positive magnitude for convenience
    float m_lineGap = 0.f;

    // Atlas
    GLuint m_texture = 0;
    int    m_atlasSize = 1024;
    std::vector<unsigned char> m_cpuAtlas;

    // Simple shelf packer state
    int m_placeX = 0;
    int m_placeY = 0;
    int m_rowH   = 0;

    // Cache
    std::unordered_map<uint32_t, Glyph> m_glyphs;

    // ---- Impl ----
    void initFT() {
        if (FT_Init_FreeType(&m_ft)) throw std::runtime_error("FreeType init failed");
        if (FT_New_Face(m_ft, m_fontPath.c_str(), 0, &m_face))
            throw std::runtime_error("FreeType: load face failed: " + m_fontPath);

        // Set pixel height
        if (FT_Set_Pixel_Sizes(m_face, 0, m_pixelHeight))
            throw std::runtime_error("FreeType: set pixel sizes failed");

        // Metrics (in 26.6; convert to px)
        m_ascent  = m_face->size->metrics.ascender  / 64.0f;
        float desc = std::abs(m_face->size->metrics.descender / 64.0f);
        m_descent = desc;
        m_lineGap = (m_face->size->metrics.height / 64.0f) - (m_ascent + m_descent);
        if (m_lineGap < 0) m_lineGap = 0;
    }

    void initHB() {
        // Create hb_face/hb_font from FT_Face to inherit scaling/hinting
        m_hbFace = hb_ft_face_create_referenced(m_face);
        m_hbFont = hb_ft_font_create_referenced(m_face);

        // Ensure HarfBuzz knows pixels-per-em (already via hb-ft)
        // You can tweak load flags here if you need specific hinting.
    }

    void initGL() {
        // Create empty RED atlas
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_atlasSize, m_atlasSize, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        // Swizzle RED -> ALPHA (so shader can use .a as coverage)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);

        glBindTexture(GL_TEXTURE_2D, 0);

        // CPU atlas buffer
        m_cpuAtlas.assign((size_t)m_atlasSize * m_atlasSize, 0);

        // Shelf packing reset
        m_placeX = m_pad; m_placeY = m_pad; m_rowH = 0;
    }

    // Simple shelf packing: place (w,h) + padding; returns true & updates placement if fits.
    bool findShelfRect(int w, int h, int pad, int& outX, int& outY, int& rowH) {
        if (w <= 0 || h <= 0) { outX = outY = 0; return true; }
        int needW = w + pad * 2;
        int needH = h + pad * 2;

        if (m_placeX + needW > m_atlasSize) {
            // new row
            m_placeX = pad;
            m_placeY += rowH + pad;
            rowH = 0;
        }
        if (m_placeY + needH > m_atlasSize) {
            return false; // no space; need grow
        }
        outX = m_placeX;
        outY = m_placeY;
        m_placeX += needW;
        rowH = std::max(rowH, h + pad * 2);
        return true;
    }

    bool growAtlas() {
        // Double size (cap as needed)
        int newSize = m_atlasSize * 2;
        if (newSize > 8192) return false; // avoid runaway

        // Create new CPU buffer
        std::vector<unsigned char> bigger((size_t)newSize * newSize, 0);
        // Blit old into new (top-left aligned)
        for (int y = 0; y < m_atlasSize; ++y) {
            std::copy_n(&m_cpuAtlas[(size_t)y * m_atlasSize],
                        m_atlasSize,
                        &bigger[(size_t)y * newSize]);
        }
        m_cpuAtlas.swap(bigger);

        // Recreate GL texture and upload
        glBindTexture(GL_TEXTURE_2D, m_texture);
        // Allocate new
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, newSize, newSize, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
        // Upload old contents into same top-left region
        // Before updating m_atlasSize, store old size
        int oldSize = m_atlasSize;
        m_atlasSize = newSize;

        // Then upload with old size:
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, oldSize, oldSize, GL_RED, GL_UNSIGNED_BYTE, &m_cpuAtlas[0]);
        glBindTexture(GL_TEXTURE_2D, 0);

        m_atlasSize = newSize;

        // Reset packing cursor to continue in the same row at old position
        // (We conservatively restart a new row)
        m_placeX = m_pad;
        m_placeY = m_pad;
        m_rowH   = 0;

        // Re-pack would be ideal, but for simplicity we just continue from scratch top-left
        // and keep existing regions as-is (since we copied them). To keep a valid cursor,
        // walk over cached glyphs to find next empty position:
        int maxX = 0, maxY = 0;
        for (const auto& kv : m_glyphs) {
            const auto& g = kv.second;
            int x = int(g.uv1.x * m_atlasSize); // approx
            int y = int(g.uv1.y * m_atlasSize);
            maxX = std::max(maxX, x);
            maxY = std::max(maxY, y);
        }
        m_placeX = std::max(m_pad, maxX + m_pad);
        m_placeY = std::max(m_pad, maxY + m_pad);
        m_rowH   = m_pad * 2;

        return true;
    }

    void blitBitmapToAtlas(FT_Bitmap& bm, int dstX, int dstY) {
        // FreeType bitmap is 8-bit coverage already (for FT_LOAD_RENDER default)
        // Copy into CPU atlas (for debug or later re-upload)
        for (unsigned int row = 0; row < bm.rows; ++row) {
            const unsigned char* src = &bm.buffer[row * bm.pitch];
            unsigned char* dst = &m_cpuAtlas[(size_t)(dstY + (int)row) * m_atlasSize + dstX];
            std::copy_n(src, bm.width, dst);
        }
    }

    // Add these debug functions to your FontAtlas class

// 1. Debug: Save atlas to file to visually inspect
void saveAtlasToFile(const std::string& filename) const {
    if (m_cpuAtlas.empty()) {
        printf("Atlas is empty!\n");
        return;
    }

    // Simple PGM format
    FILE* f = fopen(filename.c_str(), "wb");
    if (!f) return;

    fprintf(f, "P5\n%d %d\n255\n", m_atlasSize, m_atlasSize);
    fwrite(m_cpuAtlas.data(), 1, m_cpuAtlas.size(), f);
    fclose(f);
    printf("Saved atlas to %s (%dx%d)\n", filename.c_str(), m_atlasSize, m_atlasSize);
}

// 2. Debug: Print detailed glyph info
void debugGlyph(uint32_t glyphIndex) {
    printf("\n=== DEBUG GLYPH %u ===\n", glyphIndex);

    if (FT_Load_Glyph(m_face, glyphIndex, FT_LOAD_RENDER)) {
        printf("ERROR: Failed to load glyph %u\n", glyphIndex);
        return;
    }

    FT_GlyphSlot slot = m_face->glyph;
    FT_Bitmap& bm = slot->bitmap;

    printf("Bitmap: %ux%u, pitch=%d, pixel_mode=%d\n",
           bm.width, bm.rows, bm.pitch, bm.pixel_mode);
    printf("Bearing: (%d, %d)\n", slot->bitmap_left, slot->bitmap_top);
    printf("Advance: %ld (26.6 fixed)\n", slot->advance.x);

    // Print a small sample of bitmap data
    printf("Bitmap data sample (first row):\n");
    for (unsigned int i = 0; i < std::min(10u, bm.width); ++i) {
        printf("%3u ", bm.buffer[i]);
    }
    printf("\n");
}

// 3. Debug: Test basic ASCII characters
void debugBasicChars() {
    printf("\n=== TESTING BASIC ASCII ===\n");
    for (char c = 'A'; c <= 'E'; ++c) {
        uint32_t glyphIdx = charToGlyphIndex(c);
        printf("'%c' -> glyph %u\n", c, glyphIdx);

        const Glyph* g = ensureGlyph(glyphIdx);
        if (g) {
            printf("  UV: (%.3f,%.3f) to (%.3f,%.3f)\n",
                   g->uv0.x, g->uv0.y, g->uv1.x, g->uv1.y);
            printf("  Size: %dx%d, Bearing: (%d,%d)\n",
                   g->size.x, g->size.y, g->bearing.x, g->bearing.y);
        } else {
            printf("  FAILED to ensure glyph!\n");
        }
    }
    saveAtlasToFile("debug_atlas.pgm");
}


// 5. Debug shaping
void debugShaping(const std::string& text) {
    printf("\n=== DEBUG SHAPING: '%s' ===\n", text.c_str());

    std::vector<ShapedGlyph> shaped;
    if (!shapeUTF8(text, shaped)) {
        printf("ERROR: Shaping failed!\n");
        return;
    }

    printf("Shaped %zu glyphs:\n", shaped.size());
    for (size_t i = 0; i < shaped.size(); ++i) {
        const auto& sg = shaped[i];
        printf("  [%zu] glyph=%u, pos=(%.2f,%.2f), advance=(%.2f,%.2f)\n",
               i, sg.glyphIndex, sg.x, sg.y, sg.advanceX, sg.advanceY);
    }

    // Test atlas population
    populateAtlasData(shaped);
    printf("After atlas population:\n");
    for (size_t i = 0; i < shaped.size(); ++i) {
        const auto& sg = shaped[i];
        printf("  [%zu] UV=(%.3f,%.3f)-(%.3f,%.3f), size=(%.1f,%.1f)\n",
               i, sg.uv0.x, sg.uv0.y, sg.uv1.x, sg.uv1.y,
               sg.sizePx.x, sg.sizePx.y);
    }
}
};

} // namespace Funccia::Graphic::GL