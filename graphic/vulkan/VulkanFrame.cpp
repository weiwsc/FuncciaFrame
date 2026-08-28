//
// Created by Wangsicong Wei on 2026-08-28.
//

#include "VulkanFrame.h"

#include "VulkanConfig.h"
#include "VulkanDevice.h"

namespace Funccia::Graphic::Vulkan {
    auto VulkanFrameController::Create(const VulkanDevice& device) -> VulkanFrameController {
        vk::CommandPoolCreateInfo poolInfo{
            .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            .queueFamilyIndex = device.queue_coordinates.graphics.QueueFamilyIndex
        };
        auto command_pool = vk::raii::CommandPool(device.logical_device, poolInfo);


        vk::CommandBufferAllocateInfo allocInfo{
            .commandPool = command_pool,
            .level = vk::CommandBufferLevel::ePrimary,
            .commandBufferCount = VulkanRenderConfig::max_frame_in_flight
        };
        auto command_buffers = vk::raii::CommandBuffers(device.logical_device, allocInfo);


        std::vector<VulkanFrameResource> frame_resources;
        frame_resources.reserve(VulkanRenderConfig::max_frame_in_flight);

        for (int i = 0; i < VulkanRenderConfig::max_frame_in_flight; ++i) {
            frame_resources.emplace_back(
                VulkanFrameResource{
                    .command_buffer = std::move(command_buffers[i]),
                    .present_complete_semaphore = {device.logical_device, vk::SemaphoreCreateInfo()},
                    .render_finished_semaphore = {device.logical_device, vk::SemaphoreCreateInfo()},
                    .in_flight_fences = {
                        device.logical_device, vk::FenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled}
                    }
                });
        }
        return {
            std::move(command_pool),
            std::move(frame_resources)
        };
    }

    auto VulkanFrameController::frame() const -> const VulkanFrameResource& {
        return frame_resources_[frame_index_];
    }

    auto VulkanFrameController::advanceFrame() {
        frame_index_ = (frame_index_ + 1) % VulkanRenderConfig::max_frame_in_flight;
    }
}
