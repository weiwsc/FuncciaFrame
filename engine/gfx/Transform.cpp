//
// Created by Wangsicong Wei on 2025-09-24.
//

#include "Transform.h"

#include <exception>
#include <stdexcept>


namespace vva::gfx {
    void Transform::translate(const vec3& translation, const Space space) {
        switch (space) {
        case Space::World:
            position_ += translation;
            break;
        case Space::Local:
            position_ += rotation_ * translation;
            break;
        }
    }

    void Transform::rotate(const vec3& axis, const float angle) {
        const quat axis_rotation = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
        rotation_ = axis_rotation * rotation_;
    }

    void Transform::rotate(const vec3& eulers, const Space space) {
        quat quaternion = glm::quat(glm::radians(eulers));
        switch (space) {
        case Space::World:
            rotation_ = quaternion * rotation_;
            break;
        case Space::Local:
            rotation_ = rotation_ * quaternion;
            break;
        }
    }

    void Transform::lookAt(const Transform& target, const vec3& up) {
        const vec3 forward = glm::normalize(target.position() - position_);
        rotation_ = glm::quatLookAt(forward, up);
    }

    void Transform::lookAt(const vec3& world_position, const vec3& up) {
        const vec3 forward = glm::normalize(world_position - position_);
        rotation_ = glm::quatLookAt(forward, up);
    }

    auto Transform::getModelMatrix() const -> mat4 {
        return glm::translate(mat4(1), position_) *
            glm::mat4_cast(rotation_) *
            glm::scale(mat4(1), scale_);
    }

    auto Transform::transformDirection(const vec3& direction) const -> vec3 {
        return rotation_ * (direction);
    }

    auto Transform::inverseTransformDirection(const vec3& direction) const -> vec3 {
        return glm::inverse(rotation_) * (direction);
    }

    void Transform::inverseTransformDirections(std::vector<vec3>& direction) const {
        glm::quat inverse_rotation = glm::inverse(rotation_);
        for (auto& dir : direction) {
            dir = inverse_rotation * dir;
        }
    }

    auto Transform::transformPoint(const vec3& point) const -> vec3 {
        return position_ + (rotation_ * (scale_ * point));
    }

    auto Transform::inverseTransformPoint(const vec3& point) const -> vec3 {
        return (glm::inverse(rotation_) * (point - position_)) / scale_;
    }

    auto Transform::transformVector(const vec3& vector) const -> vec3 {
        return scale_ * (rotation_ * (vector));
    }

    auto Transform::inverseTransformVector(const vec3& vector) const -> vec3 {
        return (glm::inverse(rotation_) * vector) / scale_;
    }
}
