//
// Created by Wangsicong Wei on 2026-08-28.
//

#include "VulkanFrame.h"

#include "VulkanConfig.h"
#include "VulkanDevice.h"
#include "core/Log.h"

namespace vva::gfx::vulkan {
    auto VulkanFrameController::create(const VulkanDevice& device) -> VulkanFrameController {
        const vk::CommandPoolCreateInfo pool_info{
            .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            .queueFamilyIndex = device.queue_coordinates.graphics.queue_family_index
        };
        auto command_pool = vk::raii::CommandPool(device.logical_device, pool_info);


        vk::CommandBufferAllocateInfo alloc_info{
            .commandPool = command_pool,
            .level = vk::CommandBufferLevel::ePrimary,
            .commandBufferCount = VulkanRenderConfig::MAX_FRAME_IN_FLIGHT
        };
        auto command_buffers = vk::raii::CommandBuffers(device.logical_device, alloc_info);


        std::vector<VulkanFrameResource> frame_resources;
        frame_resources.reserve(VulkanRenderConfig::MAX_FRAME_IN_FLIGHT);

        for (int i = 0; i < VulkanRenderConfig::MAX_FRAME_IN_FLIGHT; ++i) {
            frame_resources.emplace_back(
                VulkanFrameResource{
                    .command_buffer = std::move(command_buffers[i]),
                    .image_available_semaphore = {device.logical_device, vk::SemaphoreCreateInfo()},
                    .in_flight_fences = {
                        device.logical_device, vk::FenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled}
                    }
                });
        }
        vva_log_info("frame controller created");
        return {
            std::move(command_pool),
            std::move(frame_resources)
        };
    }

    auto VulkanFrameController::frame() const -> const VulkanFrameResource& {
        return frame_resources_[frame_index_];
    }

    auto VulkanFrameController::advanceFrame() -> void {
        frame_index_ = (frame_index_ + 1) % VulkanRenderConfig::MAX_FRAME_IN_FLIGHT;
    }
}
