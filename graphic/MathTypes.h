//
// Created by Wangsicong Wei on 2025-09-24.
//

#ifndef FUNCCIAFRAME_MATHTYPES_H
#define FUNCCIAFRAME_MATHTYPES_H
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define FF_FORWARD_NEG_Z

namespace Funccia::Graphic {
    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    using vec4 = glm::vec4;
    using quat = glm::quat;
    using mat4 = glm::mat4;

    struct Vector2 {
        static inline constexpr vec2 zero{0.f, 0.f};
        static inline constexpr vec2 one{1.f, 1.f};
        static inline constexpr vec2 up{0.f, 1.f};
        static inline constexpr vec2 down{0.f, -1.f};
        static inline constexpr vec2 left{-1.f, 0.f};
        static inline constexpr vec2 right{1.f, 0.f};
    };

    struct Vector3 {
        static inline constexpr vec3 zero{0.f, 0.f, 0.f};
        static inline constexpr vec3 one{1.f, 1.f, 1.f};
        static inline constexpr vec3 up{0.f, 1.f, 0.f};
        static inline constexpr vec3 down{0.f, -1.f, 0.f};
        static inline constexpr vec3 left{-1.f, 0.f, 0.f};
        static inline constexpr vec3 right{1.f, 0.f, 0.f};

#ifdef FF_FORWARD_NEG_Z
        static inline constexpr vec3 forward{0.f, 0.f, -1.f};
        static inline constexpr vec3 back{0.f, 0.f, 1.f};
#else
        static inline constexpr vec3 forward{0.f, 0.f, 1.f};
        static inline constexpr vec3 back{0.f, 0.f, -1.f};
#endif
    };

    struct Matrix4x4 {
    public:
        static inline constexpr mat4 identity{mat4(1.0)};
        static inline constexpr mat4 zero{mat4(0.0)};

        static inline mat4 perspective(float fov, float aspect, float zNear, float zFar) {
            return glm::perspective(glm::radians(fov), aspect, zNear, zFar);
        }

        static inline mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
            return glm::ortho(left, right, bottom, top, zNear, zFar);
        }

        static inline mat4 lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
            return glm::lookAt(eye, center, up);
        }

        static inline mat4 translate(const vec3 &translation) {
            return glm::translate(mat4(1.0), translation);
        }

        static inline mat4 rotate(float angle, const vec3 &axis) {
            return glm::rotate(mat4(1.0), glm::radians(angle), axis);
        }

        static inline mat4 scale(const vec3 &scale) {
            return glm::scale(mat4(1.0), scale);
        }

        static inline mat4 inverse(const mat4 &matrix) {
            return glm::inverse(matrix);
        }
    };
}

#endif //FUNCCIAFRAME_MATHTYPES_H
