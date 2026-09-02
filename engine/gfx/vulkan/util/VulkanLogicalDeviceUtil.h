//
// Created by Wangsicong Wei on 2026-07-27.
//

#pragma once


#include <iostream>

#include "../VulkanDevice.h"
#include "../VulkanInclude.h"

namespace vva::gfx::vulkan::util {
    auto createVulkanDevice(vk::raii::PhysicalDevice physical_device, DeviceQueueCoordinates queue_selection) -> VulkanDevice;
    auto selectDeviceQueues(const vk::raii::SurfaceKHR& surface, const vk::raii::PhysicalDevice& physical_device) -> std::optional<DeviceQueueCoordinates>;
    uint32_t findTransferQue(uint32_t graphics_index, const vk::raii::PhysicalDevice& physical_device);
}
