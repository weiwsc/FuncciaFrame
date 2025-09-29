#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "TextRenderer.h"
#include "FontAtlas.h"
#include "Shader.h"

#include <glm/common.hpp>
#include <cmath>
#include <algorithm>

namespace Funccia::Graphic::GL {

TextRenderer::TextRenderer(FontAtlas& atlas,
                           const char* vertPath,
                           const char* fragPath)
    : m_atlas(atlas) {

    m_shader.initialize(vertPath, fragPath);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // initial capacity (grows as needed)
    glBufferData(GL_ARRAY_BUFFER, 10000 * 6 * sizeof(CharVertex), nullptr, GL_DYNAMIC_DRAW);

    // layout: 0=pos vec2, 1=uv vec2, 2=color vec3
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(CharVertex), (void*)offsetof(CharVertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CharVertex), (void*)offsetof(CharVertex, uv));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CharVertex), (void*)offsetof(CharVertex, color));

    glBindVertexArray(0);

    // half-texel inset to reduce bleeding between packed glyphs
    m_halfTexelUV = 0.5f / float(m_atlas.atlasSize());

    // cache uniforms
    m_shader.use();
    m_locSampler  = glGetUniformLocation(m_shader.getID(), "fontAtlas");
    m_locViewSize = glGetUniformLocation(m_shader.getID(), "u_viewSize");
    if (m_locSampler >= 0) m_shader.setInt("fontAtlas", 0);
}

TextRenderer::~TextRenderer() {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void TextRenderer::emitGlyph(float xpos, float yposTop, float w, float h,
                             float u0, float v0, float u1, float v1,
                             const glm::vec3& color) {
    m_vertices.push_back({{xpos,     yposTop    }, {u0, v0}, color});
    m_vertices.push_back({{xpos,     yposTop+h  }, {u0, v1}, color});
    m_vertices.push_back({{xpos+w,   yposTop+h  }, {u1, v1}, color});

    m_vertices.push_back({{xpos,     yposTop    }, {u0, v0}, color});
    m_vertices.push_back({{xpos+w,   yposTop+h  }, {u1, v1}, color});
    m_vertices.push_back({{xpos+w,   yposTop    }, {u1, v0}, color});
}

void TextRenderer::queue(const std::string& text, float x, float y, float scale, const glm::vec3& color) {
    if (text.empty()) return;

    // Shape full string
    std::vector<ShapedGlyph> shaped;
    m_atlas.shapeUTF8(text, shaped);
    m_atlas.populateAtlasData(shaped);

    const float asc       = m_atlas.ascent();
    const float baselineY = std::round(y + asc * scale);
    const float baseX     = std::round(x);

    for (const auto& sg : shaped) {
        float xpos    = std::round(baseX + sg.x + sg.bearingPx.x * scale);
        float yposTop = std::round(baselineY - sg.y - sg.bearingPx.y * scale);
        float w       = std::round(sg.sizePx.x * scale);
        float h       = std::round(sg.sizePx.y * scale);

        float u0 = sg.uv0.x + m_halfTexelUV;
        float v0 = sg.uv0.y + m_halfTexelUV;
        float u1 = sg.uv1.x - m_halfTexelUV;
        float v1 = sg.uv1.y - m_halfTexelUV;

        emitGlyph(xpos, yposTop, w, h, u0, v0, u1, v1, color);
    }
}

auto TextRenderer::queue(const std::string& text,
                         float x, float y,
                         float scale,
                         const glm::vec3& color,
                         float width_limit) -> float
{
    const float asc   = m_atlas.ascent();
    const float dsc   = m_atlas.descent();
    const float gap   = m_atlas.lineGap();
    const float lineH = std::round((asc - dsc + gap) * scale);

    float baselineY = std::round(y + asc * scale);
    float startX    = std::round(x);

    if (text.empty()) {
        return std::round((asc + dsc) * scale);
    }

    // Tokenize: words/spaces/newlines (UTF-8 aware enough for ASCII + multibyte pass-through)
    struct Tok { std::string s; bool isNL=false; bool isSpace=false; };
    std::vector<Tok> toks; toks.reserve(text.size());
    {
        std::string cur; cur.reserve(32);
        auto flush = [&]{ if (!cur.empty()) { toks.push_back({cur,false,false}); cur.clear(); } };
        for (size_t i=0; i<text.size();) {
            unsigned char ch = (unsigned char)text[i];
            if (ch == '\n') { flush(); toks.push_back({"\n", true, false}); ++i; continue; }
            if (ch == ' ')  { flush(); toks.push_back({" ", false, true}); ++i; continue; }
            int adv = 1;
            if      ((ch & 0xE0) == 0xC0) adv = 2;
            else if ((ch & 0xF0) == 0xE0) adv = 3;
            else if ((ch & 0xF8) == 0xF0) adv = 4;
            cur.append(text, i, adv);
            i += adv;
        }
        flush();
    }

    auto shapeWidth = [&](const std::string& s,
                          std::vector<ShapedGlyph>& out) -> float {
        out.clear();
        if (s.empty()) return 0.f;
        m_atlas.shapeUTF8(s, out);
        float w = 0.f; for (auto& g : out) w += g.advanceX;
        return w;
    };

    std::vector<ShapedGlyph> shapedTok;

    float penX = startX;
    auto newline = [&](){ baselineY += lineH; penX = startX; };

    for (size_t i=0; i<toks.size(); ++i) {
        const Tok& t = toks[i];
        if (t.isNL) { newline(); continue; }

        if (t.isSpace) {
            if (penX == startX) continue; // skip leading space
            float spaceW = shapeWidth(t.s, shapedTok);
            if (width_limit > 0.f && (penX + spaceW - startX) > width_limit) {
                newline(); // drop trailing space
            } else {
                // advance by space (usually bitmap-empty)
                // atlas data not needed
                penX += spaceW * scale;
            }
            continue;
        }

        // Word
        float wordW = shapeWidth(t.s, shapedTok);

        if (width_limit > 0.f && (penX + wordW - startX) > width_limit && penX > startX) {
            newline();
        }

        m_atlas.populateAtlasData(shapedTok);

        const float baseX = penX;
        const float baseY = baselineY;

        for (const auto& sg : shapedTok) {
            float xpos    = std::round(baseX + sg.x + sg.bearingPx.x * scale);
            float yposTop = std::round(baseY - sg.y - sg.bearingPx.y * scale);
            float w       = std::round(sg.sizePx.x * scale);
            float h       = std::round(sg.sizePx.y * scale);

            float u0 = sg.uv0.x + m_halfTexelUV;
            float v0 = sg.uv0.y + m_halfTexelUV;
            float u1 = sg.uv1.x - m_halfTexelUV;
            float v1 = sg.uv1.y - m_halfTexelUV;

            emitGlyph(xpos, yposTop, w, h, u0, v0, u1, v1, color);
        }

        penX += wordW * scale;
    }

    // block height = bottom of last line - top y
    float blockHeight = std::round((baselineY + dsc * scale) - y);
    if (blockHeight < std::round((asc + dsc) * scale))
        blockHeight = std::round((asc + dsc) * scale);
    return blockHeight;
}

void TextRenderer::clear() {
    m_vertices.clear();
}

void TextRenderer::flush(GLFWwindow* window) {
    if (m_vertices.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    GLint curSize = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &curSize);
    GLsizeiptr need = GLsizeiptr(m_vertices.size() * sizeof(CharVertex));
    if (need > curSize) {
        glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(need * 1.5f), nullptr, GL_DYNAMIC_DRAW);
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, need, m_vertices.data());

    GLboolean depthWas = glIsEnabled(GL_DEPTH_TEST);
    if (depthWas) glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    // premultiplied alpha
    glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_atlas.texture());

    glBindVertexArray(m_vao);

    int fbw=0, fbh=0;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);

    if (m_locViewSize >= 0) {
        // You don't have setVec2 on Shader; set directly:
        glUniform2f(m_locViewSize, (float)fbw, (float)fbh);
    }
    if (m_locSampler >= 0) m_shader.setInt("fontAtlas", 0);

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_vertices.size());

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (depthWas) glEnable(GL_DEPTH_TEST);

    m_vertices.clear();
}

} // namespace Funccia::Graphic::GL