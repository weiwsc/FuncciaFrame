//
// Created by Wangsicong Wei on 2025-09-02.
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include "Mesh.h"

//#include <GLFW/glfw3.h>

#include "Shader.h"
#include "../../core/util.h"

namespace Funccia::Graphic::GL {
    Mesh::Mesh(std::vector<float> vertices, std::vector<int> indices) {
        m_vao = 0;
        m_vbo = 0;
        m_ebo = 0;
        m_verticesData = std::move(vertices);
        m_indicesData = std::move(indices);
    }

    Mesh::~Mesh() { Cleanup(); }

    void Mesh::Create() {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_verticesData.size() * sizeof(float), m_verticesData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesData.size() * sizeof(int), m_indicesData.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
    }


    void Mesh::Cleanup() {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
    }


    void Mesh::Draw(Shader *shader) {
        shader->use();
        glm::mat4 model = m_transform.getModelMatrix();
        //glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        float aspect = (float) viewport[2] / (float) viewport[3];
        //float aspect = 1.0f;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);


        glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"), 1, GL_FALSE,
                           glm::value_ptr(projection));


        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indicesData.size(), GL_UNSIGNED_INT, 0);
    }

    void Mesh::MoveUp(float amount) {
        m_transform.Translate(glm::vec3(0.0f, amount, 0.0f), Space::World);
    }

    void Mesh::MoveRight(float amount) {
        m_transform.Translate(glm::vec3(amount, 0.0f, 0.0f), Space::World);
    }

    void Mesh::MoveForward(float amount) {
        m_transform.Translate(glm::vec3(0.0f, 0.0f, amount), Space::World);
    }

    void Mesh::Rotate(float x, float y, float z) {
        m_transform.Rotate(x, y, z);
    }
}
