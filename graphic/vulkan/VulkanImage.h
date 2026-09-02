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
}
