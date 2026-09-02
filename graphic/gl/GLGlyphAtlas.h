//
// Created by Wangsicong Wei on 2025-09-25.
//
#ifndef FUNCCIAFRAME_GLYPHATLAS_H
#define FUNCCIAFRAME_GLYPHATLAS_H
// FreeType / HarfBuzz
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <hb.h>
#include <hb-ft.h>

// OpenGL
#include <map>
#include <string>
#include <unordered_set>
#include <glad/glad.h>

#include "../MathTypes.h"
#include "../text/Glyph.h"
#include "../text/HBShaper.h"

namespace vva::gfx::GL {
    struct AtlasCell;

    class GLGlyphAtlas {
    public:
        GLGlyphAtlas() = default;

        ~GLGlyphAtlas();

        void CreateAtlas(int width, int height, int maxLayers);

        void BindAtlasLayer(int layerIndex) const;

        void BindCurrentAtlasLayer() const;

        [[nodiscard]] auto GlyphExists(hb_codepoint_t codepoint) const -> bool;

        void CopyBitmapToAtlas(FT_Face face, std::unordered_set<hb_codepoint_t> &glyphToAdd);

        void WriteAtlasToDisk(int layer);


        void initFreeType();

        auto loadFreeTypeFace(const std::string &fontPath) -> FT_Face;

        FT_Face LoadFont(const std::string &fontPath);

        auto GetGlyph(hb_codepoint_t codepoint) -> AtlasCell;

        [[nodiscard]] auto GetAtlasTextureArray() const -> GLuint {return m_atlasTextureArray;};
    private:
        auto PackGlyph(int width, int height, int &x, int &y) -> bool;

        auto PackGlyphInRow(int paddedWidth, int paddedHeight, int &x, int &y) -> bool;

        auto AdvanceRow() -> bool;

        auto AdvanceLayer() -> bool;

        FT_Library m_ft{nullptr};

        GLuint m_atlasTextureArray{0};
        int m_atlasWidth{0};
        int m_atlasHeight{0};
        int m_maxAtlasLayers{0};
        int m_atlasCurrentLayerIndex{0};


        int m_cursorX{10};
        int m_cursorY{10};
        int m_maxRowHeight{0};
        int m_pad{10};

        std::map<hb_codepoint_t, AtlasCell> GlyphDictionary{};
        std::map<std::string, FT_Face> FontDictionary{};
    };

    struct AtlasCell {
        vec4 uv;
        vec2 size;
        vec2 bearing;
        int layer;
    };
}

#endif //FUNCCIAFRAME_GLYPHATLAS_H
