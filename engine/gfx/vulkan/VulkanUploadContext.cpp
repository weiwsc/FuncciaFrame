//
// Created by Wangsicong Wei on 2026-08-28.
//

#include "VulkanUploadContext.h"
#include "VulkanDevice.h"
namespace vva::gfx::vulkan{
    auto createVulkanUploadContext(const VulkanDevice& device) -> VulkanUploadContext {
        vk::CommandPoolCreateInfo pool_info_transfer{
            .flags = vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            .queueFamilyIndex = device.queue_coordinates.transfer.queue_family_index
        };
        auto transfer_command_pool = vk::raii::CommandPool(device.logical_device, pool_info_transfer);

        vk::CommandBufferAllocateInfo alloc_info_transfer{
            .commandPool = transfer_command_pool,
            .level = vk::CommandBufferLevel::ePrimary,
            .commandBufferCount = 1
        };
        auto transfer_command_buffers = vk::raii::CommandBuffers(device.logical_device, alloc_info_transfer);

        return {
            .command_pool =  std::move(transfer_command_pool),
            .command_buffer = std::move(transfer_command_buffers.front()),
            .fence = {device.logical_device, vk::FenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled}}
        };
    }
}