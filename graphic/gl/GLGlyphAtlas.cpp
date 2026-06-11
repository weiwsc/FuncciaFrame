//
// Created by Wangsicong Wei on 2025-09-25.
//

#include "GLGlyphAtlas.h"

#include <iostream>
#include <ranges>
#include <unordered_set>
#include <vector>
#include <freetype/freetype.h>


#include "../text/HBShaper.h"
#include "stb_image_write.h"

namespace Funccia::Graphic::GL {
    GLGlyphAtlas::~GLGlyphAtlas() {
        for (auto &val: FontDictionary | std::views::values) {
            FT_Done_Face(val);
        }
        if (m_ft) {
            FT_Done_FreeType(m_ft);
        }
        if (m_atlasTextureArray) {
            glDeleteTextures(1, &m_atlasTextureArray);
        }
    }

    void GLGlyphAtlas::CreateAtlas(int width, int height, int maxLayers) {
        if (m_ft == nullptr) {
            initFreeType();
        }

        m_atlasHeight = height;
        m_atlasWidth = width;
        m_maxAtlasLayers = maxLayers;

        glGenTextures(1, &m_atlasTextureArray);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_atlasTextureArray);

        glTexImage3D(GL_TEXTURE_2D_ARRAY,
                     0,
                     GL_R8,
                     width, height, maxLayers,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     nullptr);


        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        m_atlasCurrentLayerIndex = 0;
        m_cursorX = m_pad;
        m_cursorY = m_pad;
        m_maxRowHeight = 0;
    }


    void GLGlyphAtlas::BindAtlasLayer(int layerIndex) const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_atlasTextureArray);
    }

    void GLGlyphAtlas::BindCurrentAtlasLayer() const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_atlasTextureArray);
    }

    auto GLGlyphAtlas::GlyphExists(hb_codepoint_t codepoint) const -> bool {
        return GlyphDictionary.contains(codepoint);
    }


    void GLGlyphAtlas::CopyBitmapToAtlas(FT_Face face, std::unordered_set<hb_codepoint_t> &glyphToAdd) {
        for (auto &result: glyphToAdd) {
            //Glyph glyph = Glyph();
            FT_Int32 flags = FT_LOAD_DEFAULT;
            FT_Load_Glyph(face, result, flags);
            FT_Error err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_SDF);
            if (err) continue;
            const FT_Bitmap &bm = face->glyph->bitmap;
            if (bm.width == 0 || bm.rows == 0 || bm.buffer == nullptr) continue;

            BindCurrentAtlasLayer();

            glBindTexture(GL_TEXTURE_2D_ARRAY, m_atlasTextureArray);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // Handle row stride (pitch)
            //glPixelStorei(GL_UNPACK_ROW_LENGTH, bm.pitch > 0 ? bm.pitch : -bm.pitch);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, bm.pitch > 0 ? bm.pitch : 0);

            const uint8_t *src = bm.buffer;
            if (bm.pitch < 0) {
                src = bm.buffer + (bm.rows - 1) * (-bm.pitch); // flip if top-down
            }
            int w = bm.width;
            int h = bm.rows;

            int startX = 0;
            int startY = 0;

            if (!PackGlyph(w, h, startX, startY)) {
                throw std::runtime_error("GLGlyphAtlas: Failed to pack glyph");
            }


            glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                            0, // level
                            startX, startY, m_atlasCurrentLayerIndex, // dest offset in atlas
                            w, h, 1,
                            GL_RED, GL_UNSIGNED_BYTE,
                            src);

            GlyphDictionary[result] = {
                .uv = {
                    static_cast<float>(startX) / static_cast<float>(m_atlasWidth),
                    static_cast<float>(startY) / static_cast<float>(m_atlasHeight),
                    static_cast<float>(startX + w) / static_cast<float>(m_atlasWidth),
                    static_cast<float>(startY + h) / static_cast<float>(m_atlasHeight)
                },
                .size = {static_cast<float>(w), static_cast<float>(h)},
                .bearing = {static_cast<float>(face->glyph->bitmap_left),
                            static_cast<float>(face->glyph->bitmap_top)},
                .layer = m_atlasCurrentLayerIndex,
            };
        }


        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // restore default
    }

    auto GLGlyphAtlas::PackGlyph(int width, int height, int &x, int &y) -> bool {
        const int pw = width + 2 * m_pad;
        const int ph = height + 2 * m_pad;

        while (!PackGlyphInRow(pw, ph, x, y)) {
            while (!AdvanceRow()) {
                if (!AdvanceLayer()) {
                    return false;
                }
            }
        }
        x += m_pad;
        y += m_pad;
        return true;
    }

    auto GLGlyphAtlas::PackGlyphInRow(int paddedWidth, int paddedHeight, int &x, int &y) -> bool {
        if (m_cursorY + paddedHeight > m_atlasHeight) return false;
        if (m_cursorX + paddedWidth <= m_atlasWidth) {
            x = m_cursorX;
            y = m_cursorY;
            m_cursorX += paddedWidth;
            m_maxRowHeight = std::max(m_maxRowHeight, paddedHeight);
            return true;
        }
        return false;
    }

    auto GLGlyphAtlas::AdvanceRow() -> bool {
        if (m_cursorY + m_maxRowHeight < m_atlasHeight) {
            m_cursorX = m_pad;
            m_cursorY += m_maxRowHeight;
            m_maxRowHeight = 0;
            return true;
        }
        return false;
    }

    auto GLGlyphAtlas::AdvanceLayer() -> bool {
        if (m_atlasCurrentLayerIndex + 1 >= m_maxAtlasLayers) {
            return false; // no more pages available
        }
        m_atlasCurrentLayerIndex += 1;

        m_cursorX = m_pad;
        m_cursorY = m_pad;
        m_maxRowHeight = 0;
        return true;
    }


    void GLGlyphAtlas::WriteAtlasToDisk(int layer) {
        GLuint fbo = 0;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  m_atlasTextureArray, /*level*/0, /*layer*/layer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDeleteFramebuffers(1, &fbo);
            return;
        }

        std::vector<uint8_t> pixels(m_atlasWidth * m_atlasHeight);
        glReadPixels(0, 0, m_atlasWidth, m_atlasHeight, GL_RED, GL_UNSIGNED_BYTE, pixels.data());

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &fbo);

        stbi_write_png("atlas_layer.png", m_atlasWidth, m_atlasHeight, 1, pixels.data(), m_atlasWidth);
    }


    void GLGlyphAtlas::initFreeType() {
        if (FT_Init_FreeType(&m_ft)) {
            throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
        }
    }

    auto GLGlyphAtlas::loadFreeTypeFace(const std::string &fontPath) -> FT_Face {
        FT_Face face;
        if (FT_New_Face(m_ft, fontPath.c_str(), 0, &face)) {
            throw std::runtime_error("ERROR::FREETYPE: Failed to load font: " + fontPath);
        }
        FT_Set_Pixel_Sizes(face, 96, 96);
        return face;
    }

    auto GLGlyphAtlas::LoadFont(const std::string &fontPath) -> FT_Face {
        if (FontDictionary.find(fontPath) == FontDictionary.end()) {
            FontDictionary[fontPath] = loadFreeTypeFace(fontPath);
        }
        return FontDictionary[fontPath];
    }

    auto GLGlyphAtlas::GetGlyph(hb_codepoint_t codepoint) -> AtlasCell { return GlyphDictionary[codepoint]; }
}
