//
// Created by Wangsicong Wei on 2025-09-26.
//

#ifndef FUNCCIAFRAME_UIRENDER_H
#define FUNCCIAFRAME_UIRENDER_H
// UIRenderer.h
#pragma once
#include <vector>
#include <glad/glad.h>

#include "Shader.h"
#include "glm/glm.hpp"

namespace Funccia::Graphic::GL {
    struct UIInstanceData {
        float data[32];
    };
    class UIRenderer {
    private:
        Shader m_shader;
        GLuint m_VAO, m_VBO, m_instanceVBO;
        std::vector<float> instanceData;

        void CreateGeometry();

    public:
        UIRenderer();

        ~UIRenderer();

        bool Initialize(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);

        void Cleanup();

        void Render(const glm::mat4 &viewProjection, const glm::vec2 &screenSize);

        void Collect(std::initializer_list<float> values);
        void CollectStruct(UIInstanceData data);
        void BeginFrame();
    };
}
#endif //FUNCCIAFRAME_UIRENDER_H