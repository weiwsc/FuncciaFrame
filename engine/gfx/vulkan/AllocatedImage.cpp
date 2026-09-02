//
// Created by Wangsicong Wei on 2026-08-29.
//

#include "AllocatedImage.h"

#include "VulkanAllocator.h"

namespace vva::gfx::vulkan{
    AllocatedImage::AllocatedImage(VmaAllocator allocator, vk::Image image, VmaAllocation allocation) :
        allocator_(allocator), image_(image), allocation_(allocation) {
    }

    AllocatedImage::AllocatedImage(AllocatedImage&& other) noexcept :
        allocator_(std::exchange(other.allocator_, VK_NULL_HANDLE)),
        image_(std::exchange(other.image_, nullptr)),
        allocation_(std::exchange(other.allocation_, VK_NULL_HANDLE)) {
    }

    auto AllocatedImage::operator=(AllocatedImage&& other) noexcept -> AllocatedImage& {
        if (this != &other) {
            reset();
            allocator_ = std::exchange(other.allocator_, VK_NULL_HANDLE);
            image_ = std::exchange(other.image_, nullptr);
            allocation_ = std::exchange(other.allocation_, VK_NULL_HANDLE);
        }
        return *this;
    }

    auto AllocatedImage::create(vk::ImageCreateInfo image_create_info, VmaAllocator& allocator) -> AllocatedImage {
        VmaAllocationCreateInfo allocation_info{
            .usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE
        };

        VkImage raw_image = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;

        const VkResult result = vmaCreateImage(
            allocator,
            &*image_create_info,
            &allocation_info,
            &raw_image,
            &allocation,
            nullptr);

        if (result != VK_SUCCESS) {
            throw std::runtime_error(
                "vmaCreateImage failed: " +
                vk::to_string(static_cast<vk::Result>(result)));
        }

        return {
            allocator,
            vk::Image{raw_image},
            allocation
        };
    }

    auto AllocatedImage::reset() noexcept -> void {
        if (allocator_ && image_) {
            vmaDestroyImage(allocator_, static_cast<VkImage>(image_), allocation_);
        }
        allocator_ = VK_NULL_HANDLE;
        image_ = nullptr;
        allocation_ = VK_NULL_HANDLE;
    }
}
