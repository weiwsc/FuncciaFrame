//
// Created by Wangsicong Wei on 2025-09-25.
//

#include "TextRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <algorithm>


namespace Funccia::Graphic::GL {

TextRenderer::TextRenderer(const FontAtlas& atlas,
                           const char* vertPath,
                           const char* fragPath)
: m_atlas(atlas), m_shader() {
    m_shader.initialize(vertPath, fragPath);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 10000 * 6 * sizeof(CharVertex), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0); // pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(CharVertex), (void*)offsetof(CharVertex, pos));
    glEnableVertexAttribArray(1); // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CharVertex), (void*)offsetof(CharVertex, uv));
    glEnableVertexAttribArray(2); // color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CharVertex), (void*)offsetof(CharVertex, color));

    glBindVertexArray(0);

    m_halfTexelUV = 0.5f / float(m_atlas.atlasSize());

    // shader static state: sampler = 0
    m_shader.use();
    int loc = glGetUniformLocation(m_shader.getID(), "fontAtlas");
    if (loc >= 0) glUniform1i(loc, 0);
}

TextRenderer::~TextRenderer() {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void TextRenderer::setProjection(int width, int height) {
    glm::mat4 P = glm::ortho(0.0f, float(width), 0.0f, float(height));
    m_shader.use();
    int loc = glGetUniformLocation(m_shader.getID(), "projection");
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(P));
}

void TextRenderer::queue(const std::string& text, float x, float y, float scale, const glm::vec3& color) {
    float penX = std::round(x);
    float penY = std::round(y);

    for (unsigned char c : text) {
        auto g = m_atlas.glyph(c);
        if (!g) continue;

        float xpos = std::round(penX + g->bearing.x * scale);
        float ypos = std::round(penY - (g->size.y - g->bearing.y) * scale);
        float w    = std::round(g->size.x * scale);
        float h    = std::round(g->size.y * scale);

        float u0 = g->uv0.x + m_halfTexelUV;
        float v0 = g->uv0.y + m_halfTexelUV;
        float u1 = g->uv1.x - m_halfTexelUV;
        float v1 = g->uv1.y - m_halfTexelUV;

        // 2 triangles
        m_vertices.push_back({{xpos,     ypos + h}, {u0, v0}, color});
        m_vertices.push_back({{xpos,     ypos    }, {u0, v1}, color});
        m_vertices.push_back({{xpos + w, ypos    }, {u1, v1}, color});

        m_vertices.push_back({{xpos,     ypos + h}, {u0, v0}, color});
        m_vertices.push_back({{xpos + w, ypos    }, {u1, v1}, color});
        m_vertices.push_back({{xpos + w, ypos + h}, {u1, v0}, color});

        penX += (g->advance >> 6) * scale;
    }
}
void TextRenderer::queue(const std::string& text, float x, float y,
                                              float scale, const glm::vec3& color,
                                              float width_limit) {
    float penX = std::round(x);
    float penY = std::round(y);
    float startX = penX;

    // Get line height
    float lineHeight = 0;
    auto gM = m_atlas.glyph('M');
    if (gM) {
        lineHeight = (gM->size.y * scale) * 2.0f;
    } else {
        lineHeight = scale * 2.0f;
    }

    for (unsigned char c : text) {
        // Handle newlines
        if (c == '\n') {
            penX = startX;
            penY += lineHeight;
            continue;
        }

        auto g = m_atlas.glyph(c);
        if (!g) continue;

        float charWidth = (g->advance >> 6) * scale;

        // Check if character would exceed width limit
        if (width_limit > 0 && penX - startX + charWidth > width_limit && penX > startX) {
            penX = startX;
            penY -= lineHeight;
        }

        float xpos = std::round(penX + g->bearing.x * scale);
        float ypos = std::round(penY - (g->size.y - g->bearing.y) * scale);
        float w    = std::round(g->size.x * scale);
        float h    = std::round(g->size.y * scale);

        float u0 = g->uv0.x + m_halfTexelUV;
        float v0 = g->uv0.y + m_halfTexelUV;
        float u1 = g->uv1.x - m_halfTexelUV;
        float v1 = g->uv1.y - m_halfTexelUV;

        // 2 triangles
        m_vertices.push_back({{xpos,     ypos + h}, {u0, v0}, color});
        m_vertices.push_back({{xpos,     ypos    }, {u0, v1}, color});
        m_vertices.push_back({{xpos + w, ypos    }, {u1, v1}, color});

        m_vertices.push_back({{xpos,     ypos + h}, {u0, v0}, color});
        m_vertices.push_back({{xpos + w, ypos    }, {u1, v1}, color});
        m_vertices.push_back({{xpos + w, ypos + h}, {u1, v0}, color});

        penX += charWidth;
    }


}
void TextRenderer::clear() {
    m_vertices.clear();
}

void TextRenderer::flush() {
    if (m_vertices.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    GLint curSize = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &curSize);
    GLsizeiptr need = GLsizeiptr(m_vertices.size() * sizeof(CharVertex));
    if (need > curSize) {
        glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(need * 1.5f), nullptr, GL_DYNAMIC_DRAW);
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, need, m_vertices.data());

    // state: draw with blending, no depth
    GLboolean depthWas = glIsEnabled(GL_DEPTH_TEST);
    if (depthWas) glDisable(GL_DEPTH_TEST);

    m_shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_atlas.texture());

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_vertices.size());
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (depthWas) glEnable(GL_DEPTH_TEST);

    // keep vertices if you want to draw again; usually we clear after
    m_vertices.clear();
}
}