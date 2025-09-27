//
// Created by Wangsicong Wei on 2025-09-25.
//
/*
#include "GlyphAtlas.h"
#include <vector>
#include <freetype/freetype.h>

namespace Funccia::Graphic::GL {
    GlyphAtlas::GlyphAtlas(const std::string &fontPath, int width, int height) {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            throw std::runtime_error("GlyphAtlas: failed to initialize FreeType");
        }
        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            throw std::runtime_error("GlyphAtlas: failed to load font");
        }
        m_atlas_data = new unsigned char[width * height]();

        FT_Set_Pixel_Sizes(face, 0, 48);

        for (unsigned char c = 0; c < 128; ++c) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                // skip missing
                continue;
            }
            FT_Bitmap& bm = face->glyph->bitmap;
            Glyph g;
            g.advance = vec2(face->glyph->advance.x, face->glyph->advance.y);
            CopyBitmapToAtlas(bm, m_current_x, m_current_y, g);
            m_glyphs[c] = g;
        }
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    void GlyphAtlas::CopyBitmapToAtlas(const FT_Bitmap& bitmap, int x, int y, Glyph& g)
    {
            if (x > m_atlas_width || y > m_atlas_height ){throw std::runtime_error("GlyphAtlas: out of bounds");}

            if(x + bitmap.width > m_atlas_width) {
                x = 0;
                if (y + bitmap.rows > lowest_y) {
                    if(y + bitmap.rows > m_atlas_height) {
                        throw std::runtime_error("GlyphAtlas: out of bounds");
                    }
                    lowest_y = y + bitmap.rows;
                }
                else {y = lowest_y;}
            }


            size_t start = m_atlas_width * y + x;
            for (int i = 0; i < bitmap.rows; ++i) {
                memcpy(m_atlas_data + start + i * m_atlas_width, bitmap.buffer + i * bitmap.width, bitmap.width);
            }
            g.uv0 = vec2((float)x / m_atlas_width, (float)y / m_atlas_height);
            g.uv1 = vec2((float)(x + bitmap.width) / m_atlas_width, (float)(y + bitmap.rows) / m_atlas_height);

        m_current_x = x + bitmap.width;
        m_current_y = y + bitmap.rows;
    }
}
*/
