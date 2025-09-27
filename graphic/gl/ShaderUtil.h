//
// Created by Wangsicong Wei on 2025-09-05.
//

#ifndef FUNCCIAFRAME_SHADERUTIL_H
#define FUNCCIAFRAME_SHADERUTIL_H
#include <cstddef>

#include "glad/glad.h"

namespace Funccia::Core {
    class Asset;
}

namespace Funccia::Graphic::GL {

    enum class ShaderType {
        Vertex,
        Fragment
    };

    class ShaderUtil {
    public:
        static auto CompileVertexShader(const char *source) -> unsigned int;
        static auto CompileFragmentShader(const char *source) -> unsigned int;
        static auto CompileShader(const char *source, ShaderType type, std::size_t length = NULL) -> unsigned int;
        static auto LinkProgram(unsigned int vertexShader, unsigned int fragmentShader) -> unsigned int;
        static auto CreateShaderProgram(Core::Asset* vertexShaderSource, Core::Asset* fragmentShaderSource) -> unsigned int;
    };
}

#endif //FUNCCIAFRAME_SHADERUTIL_H