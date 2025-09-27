#include <iostream>
#include "UIRender.h"
#include "Shader.h"

namespace Funccia::Graphic::GL {

UIRenderer::UIRenderer()
    : m_VAO(0), m_VBO(0), m_instanceVBO(0) {
}

UIRenderer::~UIRenderer() {
    Cleanup();
}

bool UIRenderer::Initialize(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    // Initialize shader
    m_shader.initialize(vertexShaderPath, fragmentShaderPath);

    CreateGeometry();
    return true;
}

void UIRenderer::CreateGeometry() {
    // Quad vertices (0,0) to (1,1)
    float quadVertices[] = {
        0.0f, 0.0f,  // bottom-left
        1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f,  // top-right

        0.0f, 0.0f,  // bottom-left
        1.0f, 1.0f,  // top-right
        0.0f, 1.0f   // top-left
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_instanceVBO);

    glBindVertexArray(m_VAO);

    // Quad vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Instance data buffer setup - 12 floats per instance (3 vec4s)
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);

    const size_t instanceSize = 12 * sizeof(float); // 3 vec4s
    size_t offset = 0;

    // a_borderBox (location 1)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, instanceSize, (void*)offset);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
    offset += 4 * sizeof(float);

    // a_backgroundColor (location 2)
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, instanceSize, (void*)offset);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
    offset += 4 * sizeof(float);

    // a_borderRadius (location 3)
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, instanceSize, (void*)offset);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
}

void UIRenderer::Render(const glm::mat4& viewProjection, const glm::vec2& screenSize) {
    if (instanceData.empty()) return;

    const size_t floatsPerInstance = 12;
    const size_t instanceCount = instanceData.size() / floatsPerInstance;

    if (instanceData.size() % floatsPerInstance != 0) {
        std::cerr << "Warning: Instance data size not divisible by " << floatsPerInstance << std::endl;
        return;
    }

    // Save current state
    GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    GLboolean blendEnabled = glIsEnabled(GL_BLEND);

    m_shader.use();

    // Set uniforms
    glUniformMatrix4fv(glGetUniformLocation(m_shader.getID(), "u_viewProjection"), 1, GL_FALSE, &viewProjection[0][0]);

    // Upload instance data
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 instanceData.size() * sizeof(float),
                 instanceData.data(),
                 GL_DYNAMIC_DRAW);

    // Set UI rendering state
    glBindVertexArray(m_VAO);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instanceCount);

    // Restore previous state
    if (depthTestEnabled) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    if (cullFaceEnabled) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);

    if (blendEnabled) glEnable(GL_BLEND);
    else glDisable(GL_BLEND);

    glBindVertexArray(0);
}

void UIRenderer::Collect(std::initializer_list<float> data) {
    instanceData.insert(instanceData.end(), data.begin(), data.end());
}

void UIRenderer::BeginFrame() {
    instanceData.clear();
}

void UIRenderer::Cleanup() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_instanceVBO) glDeleteBuffers(1, &m_instanceVBO);
}

}