//
// Created by Wangsicong Wei on 2026-07-27.
//

#pragma once


#include <iostream>

#include "../VulkanDevice.h"
#include "../VulkanInclude.h"

namespace Funccia::Graphic::Vulkan::Util {
    auto CreateVulkanDevice(vk::raii::PhysicalDevice physicalDevice, DeviceQueueCoordinates queue_selection) -> VulkanDevice;
    auto SelectDeviceQueues(const vk::raii::SurfaceKHR& surface, const vk::raii::PhysicalDevice& physical_device) -> std::optional<DeviceQueueCoordinates>;
    uint32_t findTransferQue(uint32_t graphicsIndex, const vk::raii::PhysicalDevice& physicalDevice);
}
