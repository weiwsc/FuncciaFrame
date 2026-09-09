//
// Created by Wangsicong Wei on 2026-07-27.
//

#pragma once
#include "VulkanAllocator.h"
#include "VulkanInclude.h"
#include "types/VulkanTypesDef.h"

namespace vva::gfx::vulkan{
    struct VulkanInstance;

    struct VulkanDevice {
        vk::raii::PhysicalDevice physical_device{nullptr};
        vk::raii::Device logical_device{nullptr};
        DeviceQueues queues;
        DeviceQueueCoordinates queue_coordinates;
        vk::PhysicalDeviceLimits limits {};
    };
}