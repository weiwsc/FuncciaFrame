//
// Created by Wangsicong Wei on 2025-09-26.
//

#ifndef FUNCCIAFRAME_GLUIRENDERER_H
#define FUNCCIAFRAME_GLUIRENDERER_H

#pragma once
#include <vector>
#include <glad/glad.h>

#include "../RendererInterfaces.h"
#include "Shader.h"
#include "glm/glm.hpp"

namespace Funccia::Graphic::GL {
    class GLUiRenderer : public IUiRenderer {
    private:
        Shader m_shader;
        GLuint m_VAO, m_VBO, m_instanceVBO;
        std::vector<float> instanceData;

        void CreateGeometry();
        void Collect(std::initializer_list<float> values);

    public:
        GLUiRenderer();

        ~GLUiRenderer() override;

        auto Initialize(const std::string &vertexShaderPath,
                        const std::string &fragmentShaderPath) -> bool override;

        void Cleanup();

        auto Submit(const UiDrawRect& rect) -> void override;

        auto Render(const glm::vec2 &screenSize) -> void override;

        auto BeginFrame() -> void override;
    };
}
#endif //FUNCCIAFRAME_GLUIRENDERER_H
