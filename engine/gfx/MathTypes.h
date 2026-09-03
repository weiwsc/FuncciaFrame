//
// Created by Wangsicong Wei on 2025-09-24.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>



namespace vva::gfx {
    using vec2 = glm::vec2;
    using ivec2 = glm::ivec2;
    using vec3 = glm::vec3;
    using vec4 = glm::vec4;
    using quat = glm::quat;
    using mat4 = glm::mat4;

    struct Vector2 {
        static inline constexpr vec2 ZERO{0.f, 0.f};
        static inline constexpr vec2 ONE{1.f, 1.f};
        static inline constexpr vec2 UP{0.f, 1.f};
        static inline constexpr vec2 DOWN{0.f, -1.f};
        static inline constexpr vec2 LEFT{-1.f, 0.f};
        static inline constexpr vec2 RIGHT{1.f, 0.f};
    };

    struct Vector3 {
        static constexpr vec3 Z_POSITIVE {0.f, 0.f, 1.f};
        static constexpr vec3 Z_NEGATIVE {0.f, 0.f, -1.f};

        static constexpr vec3 X_POSITIVE {1.f, 0.f, 0.f};
        static constexpr vec3 X_NEGATIVE {-1.f, 0.f, 0.f};

        static constexpr vec3 Y_POSITIVE {0.f, 1.f, 0.f};
        static constexpr vec3 Y_NEGATIVE {0.f, -1.f, 0.f};

        static constexpr vec3 ZERO{0.f, 0.f, 0.f};
        static constexpr vec3 ONE{1.f, 1.f, 1.f};

        static constexpr vec3 UP{Z_POSITIVE};
        static constexpr vec3 DOWN{Z_NEGATIVE};
        static constexpr vec3 LEFT{X_NEGATIVE};
        static constexpr vec3 RIGHT{X_POSITIVE};

        static constexpr vec3 FORWARD{Y_NEGATIVE};
        static constexpr vec3 BACK{Y_POSITIVE};

    };

    struct Matrix4x4 {
    public:
        static inline constexpr mat4 IDENTITY{mat4(1.0)};
        static inline constexpr mat4 ZERO{mat4(0.0)};

        static inline auto perspective(float fov, float aspect, float z_near, float z_far) -> mat4 {
            return glm::perspective(glm::radians(fov), aspect, z_near, z_far);
        }

        static inline auto ortho(float left, float right, float bottom, float top, float z_near, float z_far) -> mat4 {
            return glm::ortho(left, right, bottom, top, z_near, z_far);
        }

        static inline auto lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) -> mat4 {
            return glm::lookAt(eye, center, up);
        }

        static inline auto translate(const vec3 &translation) -> mat4 {
            return glm::translate(mat4(1.0), translation);
        }

        static inline auto rotate(float angle, const vec3 &axis) -> mat4 {
            return glm::rotate(mat4(1.0), glm::radians(angle), axis);
        }

        static inline auto scale(const vec3 &scale) -> mat4 {
            return glm::scale(mat4(1.0), scale);
        }

        static inline auto inverse(const mat4 &matrix) -> mat4 {
            return glm::inverse(matrix);
        }
    };
}


