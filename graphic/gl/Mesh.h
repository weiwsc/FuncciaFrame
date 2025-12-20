//
// Created by Wangsicong Wei on 2025-09-02.
//

#ifndef FUNCCIAFRAME_MESH_H
#define FUNCCIAFRAME_MESH_H
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>

#include "Transform.h"

namespace Funccia::Graphic::GL {
    class Shader;

    class Mesh {
    public:
        Mesh(std::vector<float> vertices, std::vector<int> indices);

        virtual ~Mesh();

        void Create();

        void Cleanup();

        void Draw(Shader *shader);

        void MoveUp(float amount);

        void MoveRight(float amount);

        void MoveForward(float amount);

        void Rotate(float x, float y, float z);

        Transform &GetTransform() { return m_transform; }

    private:
        std::vector<float> m_verticesData;
        std::vector<int> m_indicesData;
        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ebo;
        Transform m_transform;
    };
}

#endif //FUNCCIAFRAME_MESH_H