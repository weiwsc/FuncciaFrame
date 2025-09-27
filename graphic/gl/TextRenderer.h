//
// Created by Wangsicong Wei on 2025-09-25.
//
#pragma once
#include "FontAtlas.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "GlyphAtlas.h"

#ifndef FUNCCIAFRAME_TEXTRENDERER_H
#define FUNCCIAFRAME_TEXTRENDERER_H

namespace Funccia::Graphic::GL {
    class TextRenderer {
    public:
        TextRenderer(const FontAtlas& atlas,
                     const char* vertPath,
                     const char* fragPath);

        ~TextRenderer();

        void setProjection(int width, int height); // call on init & resize
        void queue(const std::string& text, float x, float y, float scale, const glm::vec3& color);
        void queue(const std::string& text, float x, float y, float scale, const glm::vec3& color, float width_limit);
        void clear();
        void flush(); // draw all queued text

    private:
        struct CharVertex {
            glm::vec2 pos;
            glm::vec2 uv;
            glm::vec3 color;
        };

        const FontAtlas& m_atlas;
        Funccia::Graphic::GL::Shader m_shader;
        unsigned int m_vao = 0, m_vbo = 0;
        std::vector<CharVertex> m_vertices;
        float m_halfTexelUV; // 0.5 / atlasSize
        };
}

#endif //FUNCCIAFRAME_TEXTRENDERER_H