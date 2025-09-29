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
#include <glad/glad.h>

#include "../gl/MathTypes.h"
#include "Glyph.h"
namespace Funccia::Graphic::GL {

    class GlyphAtlas {
    public:
        GlyphAtlas() = default;
        ~GlyphAtlas() = default;
        void CreateAtlas(const std::string& fontPath, int width, int height);
        void CopyBitmapToAtlas(const FT_Bitmap& bitmap, int x, int y, Glyph& g);
        void WriteAtlasToDisk();

        void initFreeType();
        auto loadFreeTypeFace(const std::string &fontPath) -> FT_Face;
        void Shape();
    private:
        std::map<unsigned char, Glyph> m_glyphs;

        FT_Library m_ft {0};
        FT_Face m_face {0};



        GLuint m_atlasTexture {0};
        int m_atlasWidth {0};
        int m_atlasHeight {0};
    };

}

#endif //FUNCCIAFRAME_GLYPHATLAS_H


