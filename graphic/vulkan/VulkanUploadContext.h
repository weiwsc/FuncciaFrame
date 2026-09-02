//
// Created by Wangsicong Wei on 2026-08-28.
//

#pragma once
#include "VulkanInclude.h"
namespace vva::gfx::vulkan{
    struct VulkanDevice;

    struct VulkanUploadContext {
        vk::raii::CommandPool command_pool;
        vk::raii::CommandBuffer command_buffer;
        vk::raii::Fence fence;
    };
    auto createVulkanUploadContext(const VulkanDevice& device) -> VulkanUploadContext;
}