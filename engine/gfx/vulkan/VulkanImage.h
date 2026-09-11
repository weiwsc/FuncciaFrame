//
// Created by Wangsicong Wei on 2026-08-28.
//

#pragma once
#include "VulkanInclude.h"

namespace vva::gfx::vulkan{
    struct VulkanSwapChainImage {
        //just a non owning handle of the image, the image is owned by the swap chain
        vk::Image image;
        vk::raii::ImageView image_view;
        vk::raii::Semaphore render_finished_semaphore;
    };

    struct BarrierImageTransitionConfig {
        const vk::ImageLayout old_layout;
        const vk::ImageLayout new_layout;
        const vk::AccessFlags2 src_access_mask;
        const vk::AccessFlags2 dst_access_mask;
        const vk::PipelineStageFlags2 src_stage_mask;
        const vk::PipelineStageFlags2 dst_stage_mask;
    };

    inline auto createImageView(const vk::raii::Device& device, const vk::Image& image,
                                const vk::Format format,
                                const vk::ImageAspectFlags aspect_flags) -> VulkanSwapChainImage {
        vk::ImageViewCreateInfo view_create_info{
            .image = image,
            .viewType = vk::ImageViewType::e2D,
            .format = format,
            .subresourceRange = {
                .aspectMask = aspect_flags,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };
        auto image_view = vk::raii::ImageView(device, view_create_info);
        return {
            .image = image,
            .image_view = std::move(image_view),
            .render_finished_semaphore = vk::raii::Semaphore {device, vk::SemaphoreCreateInfo()}
        };
    }

    inline void transition_image_layout(
            const vk::raii::CommandBuffer& command_buffer,
            const vk::Image image,
            BarrierImageTransitionConfig transition_config,
            const vk::ImageAspectFlags image_aspect_flags
        ) {
        vk::ImageMemoryBarrier2 barrier = {
            .srcStageMask = transition_config.src_stage_mask,
            .srcAccessMask = transition_config.src_access_mask,
            .dstStageMask = transition_config.dst_stage_mask,
            .dstAccessMask = transition_config.dst_access_mask,
            .oldLayout = transition_config.old_layout,
            .newLayout = transition_config.new_layout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = image,
            .subresourceRange = {
                .aspectMask = image_aspect_flags,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };
        const vk::DependencyInfo dependency_info = {
            .dependencyFlags = {},
            .imageMemoryBarrierCount = 1,
            .pImageMemoryBarriers = &barrier
        };
        command_buffer.pipelineBarrier2(dependency_info);
    }
}
