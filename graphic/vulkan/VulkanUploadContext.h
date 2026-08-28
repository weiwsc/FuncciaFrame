//
// Created by Wangsicong Wei on 2026-08-28.
//

#pragma once
#include "VulkanInclude.h"
namespace Funccia::Graphic::Vulkan {
    struct VulkanDevice;

    struct VulkanUploadContext {
        vk::raii::CommandPool command_pool;
        vk::raii::CommandBuffer command_buffer;
        vk::raii::Fence fence;
    };
    auto CreateVulkanUploadContext(const VulkanDevice& device) -> VulkanUploadContext;
}