#pragma once
#include "Shader.h"
#include "../text/GlyphAtlas.h"

struct GLFWwindow; // forward-declare to avoid pulling GLFW into the header

namespace Funccia::Graphic::GL {
    enum class TextWrap;
    struct ShapedGlyph;

    class TextRenderer {
    public:
        TextRenderer();
        ~TextRenderer();
        float ProcessText(float x, float y,float w, TextWrap text_wrap,vec4 clippingBox, const std::string &text, vec4 color, float pixel, const std::string &fontPath, bool dry_run = true);

        void ShapeText(const std::string &fontPath, const std::string &text, std::vector<ShapedGlyph> &shapedText);

        void PositionText(const std::vector<AtlasCell> &shapedText, float width, float height);
        auto Initialize(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) -> bool;
        void CreateGeometry();
        void Render(const glm::vec2 &screenSize);

        void Collect(std::initializer_list<float> data) {
            instanceData.insert(instanceData.end(), data.begin(), data.end());
        }

        void BeginFrame() {
            instanceData.clear();
        }

        void Cleanup() {
            if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
            if (m_VBO) glDeleteBuffers(1, &m_VBO);
            if (m_instanceVBO) glDeleteBuffers(1, &m_instanceVBO);
        }

        void DebugDrawAtlasToDisk();
    private:
        std::unique_ptr<GlyphAtlas> m_atlas;
        Shader m_shader;


        GLuint m_VAO, m_VBO, m_instanceVBO;
        std::vector<float> instanceData;
    };

    struct GlyphInstance {
        vec2 origin;   // baseline position in screen or world px
        vec2 size;     // bitmap rect (w,h)
        vec2 bearing;  // (left, top) offset
        vec4 uvRect;   // (u0,v0,u1,v1)
        vec4 color;    // rgba
        float layer;   // if using sampler2DArray (optional)
    };

    struct ShapedGlyph {
        vec4 uv;
        vec2 size;
        vec2 bearing;
        vec2 advance;
        vec2 offset;
        int layer;
    };
    enum class TextWrap {
        None,
        Word,
        Character
    };
} // namespace Funccia::Graphic::GL
