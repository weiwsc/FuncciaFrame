//
// Created by Wangsicong Wei on 2026-07-27.
//

#pragma once
#include "VulkanInclude.h"
#include "types/VulkanTypesDef.h"

namespace Funccia::Graphic::Vulkan {
    struct VulkanInstance;

    struct VulkanDevice {
        static constexpr std::array requiredDeviceExtensions = {
            vk::KHRSwapchainExtensionName,
        };
        vk::raii::PhysicalDevice physical_device{nullptr};
        vk::raii::Device logical_device{nullptr};

        DeviceQueues device_queues;
        DeviceQueueCoordinates device_queue_coordinates;
    };
}