//
// Created by Wangsicong Wei on 2026-08-31.
//

#pragma once

#include <filesystem>

#include "../resource_management/AllocatedImage.h"

namespace vva::gfx::vulkan{
    struct VulkanUploadContext;
    struct VulkanDevice;
    using Image = vva::gfx::vulkan::AllocatedImage;

    struct Texture2D {
        uint32_t slot {}; //slot in the global texture array (I guess 0 means not uploaded?)
        Image source_image;
        vk::raii::ImageView image_view;
        vk::Extent2D extent;
        vk::Format format;
    };

    auto createTextureImage(
        VmaAllocator allocator,
        const vk::raii::Device& device,
        const vk::ImageCreateInfo& image_create_info,
        vk::Format view_format, vk::ImageAspectFlags view_aspect_flags)
        -> Texture2D;

    auto loadTextureFromFile(
        VmaAllocator allocator,
        const VulkanDevice& device,
        const VulkanUploadContext& upload,
        const std::filesystem::path& path) -> Texture2D;

    // auto createTextureSampler(const vk::raii::Device& device,
    //                           const vk::raii::PhysicalDevice& physical_device) -> vk::raii::Sampler;
    //

}
