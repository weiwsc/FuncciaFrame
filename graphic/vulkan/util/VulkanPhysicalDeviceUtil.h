//
// Created by Wangsicong Wei on 2026-06-13.
//

#pragma once

#include <span>


#include "vulkan/vulkan_raii.hpp"

#include "../types/VulkanTypesDef.h"
namespace vva::gfx::vulkan::util {
    struct PickPhysicalDeviceResult {
        vk::raii::PhysicalDevice physical_device;
        DeviceQueueCoordinates queue_coordinates;
    };
    [[nodiscard]]
    auto pickPhysicalDevice(const vk::raii::Instance& instance, const vk::raii::SurfaceKHR& surface)
        -> PickPhysicalDeviceResult;
}
