//
// Created by Wangsicong Wei on 2025-09-05.
//

#include "ShaderUtil.h"

#include <iostream>

#include "../../core/Asset.h"

namespace vva::gfx::GL {
    auto ShaderUtil::CompileVertexShader(const char *source) -> unsigned int {
        return CompileShader(source, ShaderType::Vertex);
    }

    auto ShaderUtil::CompileFragmentShader(const char *source) -> unsigned int {
        return CompileShader(source, ShaderType::Fragment);
    }

    auto ShaderUtil::CompileShader(const char *source, ShaderType type, std::size_t length) -> unsigned int {
        int glShaderType;
        switch (type) {
            case ShaderType::Vertex:
                glShaderType = GL_VERTEX_SHADER;
                break;
            case ShaderType::Fragment:
                glShaderType = GL_FRAGMENT_SHADER;
                break;
        }

        unsigned int shader = glCreateShader(glShaderType);
        auto leng = static_cast<GLint>(length);
        glShaderSource(shader, 1, &source, &leng);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            const char *shaderTypeName = (glShaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
            std::cout << "ERROR::SHADER::" << shaderTypeName
                    << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        return shader;
    }

    auto ShaderUtil::LinkProgram(unsigned int vertexShader, unsigned int fragmentShader) -> unsigned int {
        // link shaders
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return shaderProgram;
    }

    auto ShaderUtil::CreateShaderProgram(core::Asset *vertexShaderSource,
                                         core::Asset *fragmentShaderSource) -> unsigned int {
        return LinkProgram(CompileShader(reinterpret_cast<char *>(vertexShaderSource->GetData()), ShaderType::Vertex,
                                         (vertexShaderSource->GetDataSize())),
                           CompileShader(reinterpret_cast<char *>(fragmentShaderSource->GetData()),
                                         ShaderType::Fragment, (fragmentShaderSource->GetDataSize())));
    }
}
