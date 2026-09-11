//
// Created by Wangsicong Wei on 2025-09-24.
//

#ifndef FUNCCIAFRAME_TRANSFORM_H
#define FUNCCIAFRAME_TRANSFORM_H
#include <vector>

#include "MathTypes.h"

namespace vva::gfx {
    enum class Space {
        World,
        Local
    };

    class Transform {
    public:
        Transform() = default;

        Transform(const Transform& other)
            : position_(other.position_),
              rotation_(other.rotation_),
              scale_(other.scale_) {}
        auto operator=(const Transform& other) ->Transform& {
            this->position_ = other.position_;
            this->rotation_ = other.rotation_;
            this->scale_ = other.scale_;
            return *this;
        }
        ~Transform() = default;

        [[nodiscard]] auto position() const -> vec3 { return position_; }
        [[nodiscard]] auto rotation() const -> quat { return rotation_; }
        [[nodiscard]] auto scale() const -> vec3 { return scale_; }

        void setPosition(const vec3 position) { position_ = position; }
        void setPositionXPlane(const vec2& position) { position_ = vec3(position, position_.z); }

        void translate(const vec3& translation, Space space = Space::Local);


        void setRotation(const quat& rotation) { rotation_ = rotation; }

        void rotate(const vec3& axis, float angle);

        void rotate(const vec3& eulers, Space space = Space::Local);

        void rotate(float x_angle, float y_angle, float z_angle, Space space = Space::Local) {
            rotate({x_angle, y_angle, z_angle}, space);
        };

        void lookAt(const Transform& target, const vec3& up = Vector3::UP);

        void lookAt(const vec3& world_position, const vec3& up = Vector3::UP);

        void setScale(const vec3& scale) { scale_ = scale; }


        [[nodiscard]] auto getModelMatrix() const -> mat4;

        void setTransform(const vec3& position, const quat& rotation, const vec3& scale) {
            position_ = position;
            rotation_ = rotation;
            scale_ = scale;
        }

#ifdef FF_FORWARD_NEG_Z
        [[nodiscard]] auto forward() const -> vec3 { return TransformDirection({0, 0, -1}); }
#else
        [[nodiscard]] auto forward() const -> vec3 { return transformDirection({0, 0, 1}); }
#endif

        [[nodiscard]] auto right() const -> vec3 { return transformDirection({1, 0, 0}); }
        [[nodiscard]] auto up() const -> vec3 { return transformDirection({0, 1, 0}); }


        [[nodiscard]] auto transformDirection(const vec3& direction) const -> vec3;

        [[nodiscard]] auto inverseTransformDirection(const vec3& direction) const -> vec3;

        void inverseTransformDirections(std::vector<vec3>& direction) const;

        [[nodiscard]] auto transformPoint(const vec3& point) const -> vec3;

        [[nodiscard]] auto inverseTransformPoint(const vec3& point) const -> vec3;

        [[nodiscard]] auto transformVector(const vec3& vector) const -> vec3;

        [[nodiscard]] auto inverseTransformVector(const vec3& vector) const -> vec3;

    private:
        vec3 position_{0};
        quat rotation_{1, 0, 0, 0};
        vec3 scale_{1};
    };
}

#endif //FUNCCIAFRAME_TRANSFORM_H
