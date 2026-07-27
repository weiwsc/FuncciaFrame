//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "../VulkanInclude.h"
#include <algorithm>
#include <vector>

namespace Funccia::Graphic::Vulkan::Util {
    auto GetRequiredLayers(const vk::raii::Context& context,
                           const bool enableValidationLayers) -> std::vector<char const*>;
}
