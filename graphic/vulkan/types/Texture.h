//
// Created by Wangsicong Wei on 2026-08-31.
//

#pragma once

#include <filesystem>

#include "../AllocatedImage.h"

namespace Funccia::Graphic::Vulkan {
    struct VulkanUploadContext;
    struct VulkanDevice;
    using Image = Funccia::Graphic::Vulkan::AllocatedImage;

    struct Texture2D {
        Image source_image;
        vk::raii::ImageView image_view;
        vk::Extent2D extent;
        vk::Format format;
    };

    auto CreateTextureImage(
        VmaAllocator allocator,
        const vk::raii::Device& device,
        const vk::ImageCreateInfo& image_create_info,
        vk::Format view_format, vk::ImageAspectFlags view_aspect_flags)
        -> Texture2D;

    auto LoadTextureFromFile(
        VmaAllocator allocator,
        const VulkanDevice& device,
        VulkanUploadContext& upload,
        const std::filesystem::path& path) -> Texture2D;

    auto createTextureSampler(const vk::raii::Device& device,
                              const vk::raii::PhysicalDevice& physical_device) -> vk::raii::Sampler;
}
