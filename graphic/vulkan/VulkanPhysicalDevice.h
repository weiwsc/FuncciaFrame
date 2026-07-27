//
// Created by Wangsicong Wei on 2026-06-13.
//

#pragma once

#include <span>
#include "vulkan/vulkan_raii.hpp"


namespace Funccia::Graphic::Vulkan::Util {
    [[nodiscard]]
    auto pickPhysicalDevice(const vk::raii::Instance& instance,
                            std::span<char const* const> requiredDeviceExtensions)
        -> vk::raii::PhysicalDevice;
}
