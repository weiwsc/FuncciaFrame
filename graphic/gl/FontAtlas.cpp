//
// Created by Wangsicong Wei on 2025-09-25.
//
#include "stb_image_write.h"

#include "FontAtlas.h"
#include <vector>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H


#include <glad/glad.h>

namespace Funccia::Graphic::GL {
    FontAtlas::FontAtlas(const std::string& fontPath, int atlasSize, int gridSize, int fontSizePx, int pad)
    : m_atlasSize(atlasSize), m_gridSize(gridSize), m_cellSize(atlasSize / gridSize),
      m_pad(pad), m_fontSizePx(fontSizePx) {
        if (!build(fontPath)) {
            std::cerr << "FontAtlas: failed to build atlas for " << fontPath << "\n";
        }
    }

    FontAtlas::~FontAtlas() {
        if (m_texture) glDeleteTextures(1, &m_texture);
    }

    const FontAtlas::Glyph* FontAtlas::glyph(unsigned char c) const {
        auto it = m_glyphs.find(c);
        return (it == m_glyphs.end()) ? nullptr : &it->second;
    }

    bool FontAtlas::build(const std::string& fontPath) {
        FT_Library ft{};
        if (FT_Init_FreeType(&ft)) {
            std::cerr << "ERROR::FREETYPE: init failed\n";
            return false;
        }

        FT_Face face{};
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            std::cerr << "ERROR::FREETYPE: load face failed: " << fontPath << "\n";
            FT_Done_FreeType(ft);
            return false;
        }

        FT_Set_Pixel_Sizes(face, 0, m_fontSizePx);

        std::vector<unsigned char> atlas(m_atlasSize * m_atlasSize, 0);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // First pass: find max ascent and descent to determine where to place baseline
        int maxAscent = 0;
        int maxDescent = 0;

        for (unsigned char c = 32; c < 128; ++c) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                continue;
            }

            // bitmap_top is the ascent for this glyph
            int ascent = face->glyph->bitmap_top;
            // descent is how far below the baseline the glyph extends
            int descent = (int)face->glyph->bitmap.rows - ascent;

            maxAscent = std::max(maxAscent, ascent);
            maxDescent = std::max(maxDescent, descent);
        }

        // Calculate where the baseline should be in each cell
        // We need space for maxAscent above and maxDescent below, plus padding
        int totalHeight = maxAscent + maxDescent;
        int availableHeight = m_cellSize - 2 * m_pad;

        // If glyphs don't fit, warn but continue
        if (totalHeight > availableHeight) {
            std::cerr << "Warning: Font size " << m_fontSizePx
                     << " needs " << totalHeight << " pixels but cell only has "
                     << availableHeight << " pixels available\n";
        }

        // Place baseline so we have room for both ascent and descent
        // The baseline is positioned from the top of the cell
        int baselineFromTop = m_pad + maxAscent;

        for (unsigned char c = 0; c < 255; ++c) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                // skip missing
                continue;
            }
            FT_Bitmap& bm = face->glyph->bitmap;

            int gx = c % m_gridSize;
            int gy = c / m_gridSize;
            int cellX = gx * m_cellSize;
            int cellY = gy * m_cellSize;

            // Calculate destination position
            // X position: center horizontally if needed, account for bearing
            int dstX = cellX + m_pad;
            // Y position: baseline minus the glyph's ascent (bitmap_top)
            int dstY = cellY + baselineFromTop - face->glyph->bitmap_top;

            // Make sure we stay within cell bounds
            if (dstY < cellY + m_pad) {
                dstY = cellY + m_pad;
            }
            if (dstX < cellX + m_pad) {
                dstX = cellX + m_pad;
            }

            // Copy glyph bitmap to atlas
            for (unsigned int row = 0; row < bm.rows; ++row) {
                for (unsigned int col = 0; col < bm.width; ++col) {
                    int aX = dstX + (int)col;
                    int aY = dstY + (int)row;

                    // Strict bounds checking - don't write into adjacent cells
                    if (aX < cellX || aX >= cellX + m_cellSize ||
                        aY < cellY || aY >= cellY + m_cellSize ||
                        aX >= m_atlasSize || aY >= m_atlasSize) {
                        continue;
                    }

                    atlas[aY * m_atlasSize + aX] = bm.buffer[row * bm.width + col];
                }
            }

            // Apply padding (edge extension for better sampling)
            auto idx = [&](int X, int Y) -> size_t {
                if (X < 0 || Y < 0 || X >= m_atlasSize || Y >= m_atlasSize) return 0;
                return size_t(Y * m_atlasSize + X);
            };

            int gx0 = dstX;
            int gy0 = dstY;
            int gw = (int)bm.width;
            int gh = (int)bm.rows;

            // Clamp dimensions to cell boundaries
            int maxWidth = (cellX + m_cellSize - m_pad) - dstX;
            int maxHeight = (cellY + m_cellSize - m_pad) - dstY;
            gw = std::min(gw, maxWidth);
            gh = std::min(gh, maxHeight);

            // Horizontal padding: extend left/right edges
            for (int y = 0; y < gh; ++y) {
                if (gy0 + y >= cellY + m_cellSize) break;

                unsigned char L = (y < (int)bm.rows && bm.width > 0) ? bm.buffer[y * bm.width] : 0;
                unsigned char R = (y < (int)bm.rows && bm.width > 0) ? bm.buffer[y * bm.width + (bm.width - 1)] : 0;

                // Left padding
                for (int p = 1; p <= m_pad; ++p) {
                    int px = gx0 - p;
                    if (px >= cellX && px < cellX + m_cellSize) {
                        atlas[idx(px, gy0 + y)] = L;
                    }
                }

                // Right padding
                for (int p = 1; p <= m_pad; ++p) {
                    int px = gx0 + gw - 1 + p;
                    if (px >= cellX && px < cellX + m_cellSize) {
                        atlas[idx(px, gy0 + y)] = R;
                    }
                }
            }

            // Vertical padding: extend top/bottom edges
            int padXStart = std::max(cellX, gx0 - m_pad);
            int padXEnd = std::min(cellX + m_cellSize - 1, gx0 + gw - 1 + m_pad);

            for (int x = padXStart; x <= padXEnd; ++x) {
                int localX = x - gx0;
                unsigned char T = (localX >= 0 && localX < gw && gh > 0) ? atlas[idx(x, gy0)] : 0;
                unsigned char B = (localX >= 0 && localX < gw && gh > 0) ? atlas[idx(x, gy0 + gh - 1)] : 0;

                // Top padding
                for (int p = 1; p <= m_pad; ++p) {
                    int py = gy0 - p;
                    if (py >= cellY && py < cellY + m_cellSize) {
                        atlas[idx(x, py)] = T;
                    }
                }

                // Bottom padding
                for (int p = 1; p <= m_pad; ++p) {
                    int py = gy0 + gh - 1 + p;
                    if (py >= cellY && py < cellY + m_cellSize) {
                        atlas[idx(x, py)] = B;
                    }
                }
            }

            // Store UV coordinates for the actual glyph content (not padding)
            float u0 = float(dstX) / m_atlasSize;
            float v0 = float(dstY) / m_atlasSize;
            float u1 = float(dstX + bm.width) / m_atlasSize;
            float v1 = float(dstY + bm.rows) / m_atlasSize;

            m_cpuAtlas = atlas;
            Glyph g;
            g.uv0     = {u0, v0};
            g.uv1     = {u1, v1};
            g.size    = {int(bm.width), int(bm.rows)};
            g.bearing = {face->glyph->bitmap_left, face->glyph->bitmap_top};
            g.advance = face->glyph->advance.x;
            m_glyphs[c] = g;
        }

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_atlasSize, m_atlasSize, 0, GL_RED, GL_UNSIGNED_BYTE, atlas.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        // swizzle: RED -> ALPHA
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);

        glBindTexture(GL_TEXTURE_2D, 0);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return true;
    }

    // helper to draw a pixel into RGBA buffer
    static inline void putPixel(std::vector<unsigned char>& rgba, int W, int X, int Y,
                                unsigned char r, unsigned char g, unsigned char b, unsigned char a=255) {
        if (X < 0 || Y < 0 || X >= W || Y >= W) return;
        size_t i = (size_t(Y) * W + X) * 4;
        rgba[i+0]=r; rgba[i+1]=g; rgba[i+2]=b; rgba[i+3]=a;
    }

    bool FontAtlas::saveDebugPNG(const std::string& path, bool showGrid, bool showGlyphBoxes) const {
        if (m_cpuAtlas.empty()) return false;

        const int W = m_atlasSize, H = m_atlasSize;

        // visualize as RGBA: grayscale background from atlas, alpha = atlas
        std::vector<unsigned char> rgba(size_t(W)*H*4, 0);
        for (int y=0; y<H; ++y) {
            for (int x=0; x<W; ++x) {
                unsigned char a = m_cpuAtlas[size_t(y)*W + x];
                unsigned char g = a; // gray
                size_t i = (size_t(y)*W + x)*4;
                rgba[i+0] = g;
                rgba[i+1] = g;
                rgba[i+2] = g;
                rgba[i+3] = 255; // opaque for visibility in viewers
            }
        }

        // optional: draw grid lines in RED
        if (showGrid) {
            for (int gy=0; gy<=m_gridSize; ++gy) {
                int y = gy * m_cellSize;
                if (y>=H) y = H-1;
                for (int x=0; x<W; ++x) putPixel(rgba, W, x, y, 255, 0, 0);
            }
            for (int gx=0; gx<=m_gridSize; ++gx) {
                int x = gx * m_cellSize;
                if (x>=W) x = W-1;
                for (int y=0; y<H; ++y) putPixel(rgba, W, x, y, 255, 0, 0);
            }
        }

        // optional: draw glyph rectangles (UV boxes) in GREEN
        if (showGlyphBoxes) {
            auto drawRect = [&](int x0,int y0,int x1,int y1){
                x0 = std::clamp(x0,0,W-1); y0 = std::clamp(y0,0,H-1);
                x1 = std::clamp(x1,0,W-1); y1 = std::clamp(y1,0,H-1);
                for (int x=x0; x<=x1; ++x){ putPixel(rgba,W,x,y0,0,255,0); putPixel(rgba,W,x,y1,0,255,0); }
                for (int y=y0; y<=y1; ++y){ putPixel(rgba,W,x0,y,0,255,0); putPixel(rgba,W,x1,y,0,255,0); }
            };
            for (const auto& kv : m_glyphs) {
                const Glyph& g = kv.second;
                int x0 = int(std::round(g.uv0.x * W));
                int y0 = int(std::round(g.uv0.y * H));
                int x1 = int(std::round(g.uv1.x * W)) - 1;
                int y1 = int(std::round(g.uv1.y * H)) - 1;
                drawRect(x0,y0,x1,y1);
            }
        }

        // write PNG
        if (stbi_write_png(path.c_str(), W, H, 4, rgba.data(), W*4) == 0) {
            return false;
        }
        return true;
    }
}