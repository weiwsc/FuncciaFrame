//
// Created by Wangsicong Wei on 2026-07-27.
//

#pragma once


#include <iostream>

#include "../VulkanDevice.h"
#include "../VulkanInclude.h"

namespace Funccia::Graphic::Vulkan::Util {
    auto CreateVulkanDevice(const vk::raii::SurfaceKHR& surface, vk::raii::PhysicalDevice physicalDevice) -> VulkanDevice;

    uint32_t findTransferQue(uint32_t graphicsIndex, const vk::raii::PhysicalDevice& physicalDevice);
}
