//
// Created by Wangsicong Wei on 2026-09-04.
//

#pragma once
#include "gfx/Transform.h"


namespace vva::gfx::vulkan {
    struct Camera {
        Transform transform;
        glm::mat4 projection;
        [[nodiscard]] auto lookingAt() const -> glm::mat4 {
            return glm::inverse( transform.getModelMatrix());
        };
    };
}
