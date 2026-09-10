//
// Created by Wangsicong Wei on 2026-08-29.
//

#pragma once
#include "../VulkanInclude.h"
#include "vk_mem_alloc.h"

namespace vva::gfx::vulkan{
    struct VulkanAllocator;

    struct AllocatedImage {
        AllocatedImage(VmaAllocator allocator, vk::Image buffer, VmaAllocation allocation);

        AllocatedImage(const AllocatedImage&) = delete;
        auto operator=(const AllocatedImage&) -> AllocatedImage& = delete;

        AllocatedImage(AllocatedImage&& other) noexcept;

        auto operator=(AllocatedImage&& other) noexcept -> AllocatedImage& ;

        ~AllocatedImage() { reset(); }

        [[nodiscard]]
        auto handle() const noexcept -> vk::Image {
            return image_;
        }

        [[nodiscard]]
        auto allocation() const noexcept -> VmaAllocation {
            return allocation_;
        }
        static auto create(vk::ImageCreateInfo image_create_info, VmaAllocator& allocator) -> AllocatedImage;
    private:
        void reset() noexcept;
        VmaAllocator allocator_ = VK_NULL_HANDLE;
        vk::Image image_;
        VmaAllocation allocation_ = VK_NULL_HANDLE;
    };
}
