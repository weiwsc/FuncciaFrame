//
// Created by Wangsicong Wei on 2025-09-24.
//

#ifndef FUNCCIAFRAME_TRANSFORM_H
#define FUNCCIAFRAME_TRANSFORM_H
#include <vector>

#include "MathTypes.h"

namespace Funccia::Graphic::GL {
    enum class Space {
        World,
        Local
    };

    class Transform {
    public:
        Transform() = default;

        ~Transform() = default;

        [[nodiscard]] vec3 position() const { return m_position; }
        [[nodiscard]] quat rotation() const { return m_rotation; }
        [[nodiscard]] vec3 scale() const { return m_scale; }

        void setPosition(const vec3 &_position) { m_position = _position; }
        void setPositionXPlane(const vec2 &_position) { m_position = vec3(_position, m_position.z); }

        void Translate(const vec3 &_translation, Space space = Space::Local);


        void setRotation(const quat &_rotation) { m_rotation = _rotation; }

        void Rotate(const vec3 &axis, float angle);

        void Rotate(const vec3 &eulers, Space space = Space::Local);

        void Rotate(float xAngle, float yAngle, float zAngle, Space space = Space::Local) {
            Rotate({xAngle, yAngle, zAngle}, space);
        };

        void LookAt(const Transform &target, const vec3 &up = {0, 1, 0});

        void LookAt(const vec3 &worldPosition, const vec3 &up = {0, 1, 0});

        void setScale(const vec3 &_scale) { m_scale = _scale; }


        auto getModelMatrix() const -> mat4;

        void setTransform(const vec3 &_position, const quat &_rotation, const vec3 &_scale) {
            m_position = _position;
            m_rotation = _rotation;
            m_scale = _scale;
        }

#ifdef FF_FORWARD_NEG_Z
        [[nodiscard]] auto forward() const -> vec3 { return TransformDirection({0, 0, -1}); }
#else
        [[nodiscard]] auto forward() const -> vec3 { return TransformDirection({0, 0, 1}); }
#endif

        [[nodiscard]] auto right() const -> vec3 { return TransformDirection({1, 0, 0}); }
        [[nodiscard]] auto up() const -> vec3 { return TransformDirection({0, 1, 0}); }


        auto TransformDirection(const vec3 &_direction) const -> vec3;

        [[nodiscard]] auto InverseTransformDirection(const vec3 &_direction) const -> vec3;

        void InverseTransformDirections(std::vector<vec3> &_direction) const;

        auto TransformPoint(const vec3 &_point) const -> vec3;

        auto InverseTransformPoint(const vec3 &_point) const -> vec3;

        auto TransformVector(const vec3 &_vector) const -> vec3;

        auto InverseTransformVector(const vec3 &_vector) const -> vec3;

    private:
        vec3 m_position{0};
        quat m_rotation{1, 0, 0, 0};
        vec3 m_scale{1};
    };
}

#endif //FUNCCIAFRAME_TRANSFORM_H