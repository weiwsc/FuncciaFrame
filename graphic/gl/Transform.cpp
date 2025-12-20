//
// Created by Wangsicong Wei on 2025-09-24.
//

#include "Transform.h"

#include <exception>
#include <stdexcept>


namespace Funccia::Graphic::GL {
    void Transform::Translate(const vec3 &_translation, Space space) {
        switch (space) {
            case Space::World:
                m_position += _translation;
                break;
            case Space::Local:
                m_position += m_rotation * _translation;
                break;
        }
    }

    void Transform::Rotate(const vec3 &axis, float angle) {
        quat axisRotation = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
        m_rotation = axisRotation * m_rotation;
    }

    void Transform::Rotate(const vec3 &eulers, Space space) {
        quat quaternion = glm::quat(glm::radians(eulers));
        switch (space) {
            case Space::Local:
                m_rotation = quaternion * m_rotation;
                break;
            case Space::World:
                m_rotation = m_rotation * quaternion;
                break;
        }
    }

    void Transform::LookAt(const Transform &target, const vec3 &up) {
#ifdef FF_FORWARD_NEG_Z
        vec3 forward = target.position() - m_position;
        forward = glm::normalize(forward);
        vec3 right = glm::normalize(glm::cross(up, forward));
        vec3 newUp = glm::normalize(glm::cross(forward, right));
        m_rotation = glm::quatLookAt(-forward, newUp);
#else
        vec3 forward = glm::normalize(target.position() - m_position);
        vec3 right = glm::normalize(glm::cross(up, forward));
        vec3 newUp = glm::normalize(glm::cross(forward, right));
        m_rotation = glm::quatLookAt(forward, newUp);
#endif
    }

    void Transform::LookAt(const vec3 &worldPosition, const vec3 &up) {
    }

    auto Transform::getModelMatrix() const -> mat4 {
        return glm::translate(mat4(1), m_position) *
               glm::mat4_cast(m_rotation) *
               glm::scale(mat4(1), m_scale);
    }

    auto Transform::TransformDirection(const vec3 &_direction) const -> vec3 {
        return m_rotation * (_direction);
    }

    auto Transform::InverseTransformDirection(const vec3 &_direction) const -> vec3 {
        return glm::inverse(m_rotation) * (_direction);
    }

    void Transform::InverseTransformDirections(std::vector<vec3> &_direction) const {
        glm::quat inverseRotation = glm::inverse(m_rotation);
        for (auto &dir: _direction) {
            dir = inverseRotation * dir;
        }
    }

    auto Transform::TransformPoint(const vec3 &_point) const -> vec3 {
        return m_position + (m_rotation * (m_scale * _point));
    }

    auto Transform::InverseTransformPoint(const vec3 &_point) const -> vec3 {
        return (glm::inverse(m_rotation) * (_point - m_position)) / m_scale;
    }

    auto Transform::TransformVector(const vec3 &_vector) const -> vec3 {
        return m_scale * (m_rotation * (_vector));
    }

    auto Transform::InverseTransformVector(const vec3 &_vector) const -> vec3 {
        return (glm::inverse(m_rotation) * _vector) / m_scale;
    }
}