//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "../VulkanInclude.h"
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

namespace Funccia::Graphic::Vulkan::Util {
    auto GetRequiredInstanceExtensions(const vk::raii::Context& context,
                               const bool enableValidationLayers) -> std::vector<char const*>;
}
