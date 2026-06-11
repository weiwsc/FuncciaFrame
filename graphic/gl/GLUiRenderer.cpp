#include <iostream>
#include "GLUiRenderer.h"
#include "Shader.h"

namespace Funccia::Graphic::GL {
    GLUiRenderer::GLUiRenderer()
        : m_VAO(0), m_VBO(0), m_instanceVBO(0) {
    }

    GLUiRenderer::~GLUiRenderer() {
        Cleanup();
    }

    auto GLUiRenderer::Initialize(const std::string &vertexShaderPath,
                                  const std::string &fragmentShaderPath) -> bool {
        // Initialize shader
        m_shader.initialize(vertexShaderPath, fragmentShaderPath);

        CreateGeometry();
        return true;
    }

    void GLUiRenderer::CreateGeometry() {
        // Quad vertices (0,0) to (1,1)
        float quadVertices[] = {
            0.0f, 0.0f, // bottom-left
            1.0f, 0.0f, // bottom-right
            1.0f, 1.0f, // top-right

            0.0f, 0.0f, // bottom-left
            1.0f, 1.0f, // top-right
            0.0f, 1.0f // top-left
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_instanceVBO);

        glBindVertexArray(m_VAO);

        // Quad vertices
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        // Instance data buffer setup for eight vec4 attributes per rect.
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);

        const size_t instanceSize = 32 * sizeof(float);
        size_t offset = 0;

        // a_borderBox (location 1)
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 1);
        offset += 4 * sizeof(float);

        // a_backgroundColor (location 2)
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);
        offset += 4 * sizeof(float);

        // a_borderRadius (location 3)
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);
        offset += 4 * sizeof(float);

        // a_shadowProperties (location 4)
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(4);
        glVertexAttribDivisor(4, 1);
        offset += 4 * sizeof(float);

        // a_shadowColor (location 5)
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(5);
        glVertexAttribDivisor(5, 1);
        offset += 4 * sizeof(float);

        // a_borderWidths (location 6)
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(6);
        glVertexAttribDivisor(6, 1);
        offset += 4 * sizeof(float);

        // a_borderColor (location 7)
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(7);
        glVertexAttribDivisor(7, 1);
        offset += 4 * sizeof(float);

        // clipping box (location 8)
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(8);
        glVertexAttribDivisor(8, 1);
    }

    auto GLUiRenderer::Render(const glm::vec2 &screenSize) -> void {
        if (instanceData.empty()) return;

        const size_t floatsPerInstance = 32;
        const size_t instanceCount = instanceData.size() / floatsPerInstance;

        if (instanceData.size() % floatsPerInstance != 0) {
            std::cerr << "Warning: Instance data size not divisible by " << floatsPerInstance << std::endl;
            return;
        }

        m_shader.use();
        glUniform2f(glGetUniformLocation(m_shader.getID(), "u_viewSize"),
                    screenSize.x, screenSize.y);

        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);

        // Orphan the buffer to avoid stalls
        size_t bufferSize = instanceData.size() * sizeof(float);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STREAM_DRAW); // Orphan
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, instanceData.data()); // Upload

        //without orphaning
        //    glBufferData(GL_ARRAY_BUFFER,
        //            instanceData.size() * sizeof(float),
        //            instanceData.data(),
        //          GL_DYNAMIC_DRAW);

        glBindVertexArray(m_VAO);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instanceCount);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(0);
    }

    void GLUiRenderer::Collect(std::initializer_list<float> data) {
        instanceData.insert(instanceData.end(), data.begin(), data.end());
    }

    auto GLUiRenderer::Submit(const UiDrawRect& rect) -> void {
        Collect({
            rect.borderBox.x, rect.borderBox.y, rect.borderBox.z, rect.borderBox.w,
            rect.backgroundColor.r, rect.backgroundColor.g, rect.backgroundColor.b, rect.backgroundColor.a,
            rect.borderRadius.r, rect.borderRadius.g, rect.borderRadius.b, rect.borderRadius.a,
            rect.shadowProperties.x, rect.shadowProperties.y, rect.shadowProperties.z, rect.shadowProperties.w,
            rect.shadowColor.r, rect.shadowColor.g, rect.shadowColor.b, rect.shadowColor.a,
            rect.borderWidths.x, rect.borderWidths.y, rect.borderWidths.z, rect.borderWidths.w,
            rect.borderColor.r, rect.borderColor.g, rect.borderColor.b, rect.borderColor.a,
            rect.clippingBox.x, rect.clippingBox.y, rect.clippingBox.z, rect.clippingBox.w,
        });
    }

    auto GLUiRenderer::BeginFrame() -> void {
        instanceData.clear();
        instanceData.reserve(1024 * 32);
    }

    void GLUiRenderer::Cleanup() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
        if (m_instanceVBO) glDeleteBuffers(1, &m_instanceVBO);
    }
}
