//
// Created by Wangsicong Wei on 2026-08-28.
//

#include "VulkanUploadContext.h"
#include "VulkanDevice.h"
namespace Funccia::Graphic::Vulkan {
    auto CreateVulkanUploadContext(const VulkanDevice& device) -> VulkanUploadContext {
        vk::CommandPoolCreateInfo poolInfoTransfer{
            .flags = vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            .queueFamilyIndex = device.queue_coordinates.transfer.QueueFamilyIndex
        };
        auto transfer_commandPool = vk::raii::CommandPool(device.logical_device, poolInfoTransfer);

        vk::CommandBufferAllocateInfo allocInfoTransfer{
            .commandPool = transfer_commandPool,
            .level = vk::CommandBufferLevel::ePrimary,
            .commandBufferCount = 1
        };
        auto transfer_command_buffers = vk::raii::CommandBuffers(device.logical_device, allocInfoTransfer);

        return {
            .command_pool =  std::move(transfer_commandPool),
            .command_buffer = std::move(transfer_command_buffers.front()),
            .fence = {device.logical_device, vk::FenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled}}
        };
    }
}