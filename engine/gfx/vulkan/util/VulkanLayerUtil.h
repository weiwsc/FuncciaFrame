//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "../VulkanInclude.h"
#include <algorithm>
#include <vector>

namespace vva::gfx::vulkan::util {
    auto getRequiredLayers(const vk::raii::Context& context,
                           const bool enable_validation_layers) -> std::vector<char const*>;
}
