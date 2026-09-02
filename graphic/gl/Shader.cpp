//
// Created by Wangsicong Wei on 2025-09-05.
//

#include "Shader.h"

#include <glad/glad.h>

#include "ShaderUtil.h"
#include "../../core/AssetController.h"

namespace vva::gfx::GL {
    Shader::Shader() {
        m_fragmentShaderSource = nullptr;
        m_vertexShaderSource = nullptr;
    }

    Shader::~Shader() {
        glDeleteProgram(ID);
    }

    void Shader::initialize(const std::string &vertexPath, const std::string &fragmentPath) {
        m_vertexShaderSource = core::AssetController::Instance().GetAsset(vertexPath);
        m_fragmentShaderSource = core::AssetController::Instance().GetAsset(fragmentPath);
        ID = ShaderUtil::CreateShaderProgram(m_vertexShaderSource, m_fragmentShaderSource);
    }

    void Shader::use() {
        glUseProgram(ID);
    }

    void Shader::Serialize(core::Serializer &serializer) {
        serializer.write(m_vertexShaderSource);
        serializer.write(m_fragmentShaderSource);
        Resource::Serialize(serializer);
    }

    void Shader::Deserialize(core::Deserializer &deserializer) {
        deserializer.read(m_vertexShaderSource);
        deserializer.read(m_fragmentShaderSource);
        Resource::Deserialize(deserializer);
    }

    void Shader::setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    void Shader::setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
}