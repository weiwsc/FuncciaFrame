//
// Created by Wangsicong Wei on 2025-09-25.
//

#include "GlyphAtlas.h"

#include <iostream>
#include <vector>
#include <freetype/freetype.h>


#include "HBShaper.h"
#include "stb_image_write.h"

namespace Funccia::Graphic::GL {

    void GlyphAtlas::CreateAtlas(const std::string &fontPath, int width, int height) {
        initFreeType();

        m_atlasHeight = height;
        m_atlasWidth = width;

        // All functions return a value different than 0 whenever an error occurred
        glGenTextures(1, &m_atlasTexture);
        glBindTexture(GL_TEXTURE_2D, m_atlasTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0,GL_RED, GL_UNSIGNED_BYTE, nullptr); // 8-bit alpha
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        FT_Face face = loadFreeTypeFace(fontPath);

        HBShaper* shaper = new HBShaper();
        shaper->init(face);
        std::vector<HBTextInfo> results;
        shaper->Shape("Hello World!", results);

        int cursorX = 10;
        int cursorY = 10;
        for (auto& result : results) {
            //Glyph glyph = Glyph();
            FT_Int32 flags =  FT_LOAD_DEFAULT;
            FT_Load_Glyph(face, result.glyph_id, flags);
            FT_Error err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_SDF);
            if (err) continue;
            const FT_Bitmap& bm = face->glyph->bitmap;
            if (bm.width == 0 || bm.rows == 0 || bm.buffer == nullptr) continue;
            glBindTexture(GL_TEXTURE_2D, m_atlasTexture);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // Handle row stride (pitch)
            glPixelStorei(GL_UNPACK_ROW_LENGTH, bm.pitch > 0 ? bm.pitch : -bm.pitch);
            const uint8_t* src = bm.buffer;
            if (bm.pitch < 0) {
                src = bm.buffer + (bm.rows - 1) * (-bm.pitch); // flip if top-down
            }

            glTexSubImage2D(GL_TEXTURE_2D,
                            0,              // level
                            cursorX, cursorY,           // dest offset in atlas
                            bm.width, bm.rows,
                            GL_RED, GL_UNSIGNED_BYTE,
                            src);
            cursorX += bm.width + 2; // Small padding between glyphs

            // Check if we need to move to next row
            if (cursorX + 64 > width) { // Leave room for next glyph (assuming max 64px width)
                cursorX = 10;
                cursorY += 80; // Move down by max glyph height + padding
            }



            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // restore default
        }

    }

    void GlyphAtlas::CopyBitmapToAtlas(const FT_Bitmap& bitmap, int x, int y, Glyph& g)
    {

    }

    void GlyphAtlas::WriteAtlasToDisk() {
        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, m_atlasTexture, 0);

        // Read back
        std::vector<uint8_t> pixels(m_atlasWidth * m_atlasHeight);
        glReadPixels(0, 0, m_atlasWidth, m_atlasHeight,
                     GL_RED, GL_UNSIGNED_BYTE, pixels.data());

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &fbo);

        // Save
        stbi_write_png("atlas.png", m_atlasWidth, m_atlasHeight, 1, pixels.data(), m_atlasWidth);
    }

    void GlyphAtlas::initFreeType() {
        if (FT_Init_FreeType(&m_ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return;
        }
    }

    auto GlyphAtlas::loadFreeTypeFace(const std::string &fontPath) -> FT_Face {
        FT_Face face;
        if (FT_New_Face(m_ft, fontPath.c_str(), 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

        }
        //FT_Set_Char_Size(face, 0, 1000, 0, 0);
        FT_Set_Pixel_Sizes(face, 96, 96);
        return face;
    }


    void GlyphAtlas::Shape() {

    }
}

