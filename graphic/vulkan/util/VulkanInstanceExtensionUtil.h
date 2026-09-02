//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "../VulkanInclude.h"
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

namespace vva::gfx::vulkan::util {
    auto getRequiredInstanceExtensions(const vk::raii::Context& context,
                                       const bool enable_validation_layers) -> std::vector<char const*>;
}
