//
// Created by Wangsicong Wei on 2026-08-29.
//

#include "AllocatedImage.h"

namespace Funccia::Graphic::Vulkan {
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

    auto AllocatedImage::reset() noexcept -> void {
        if (allocator_ && image_) {
            vmaDestroyImage(allocator_, static_cast<VkImage>(image_), allocation_);
        }
        allocator_ = VK_NULL_HANDLE;
        image_ = nullptr;
        allocation_ = VK_NULL_HANDLE;
    }
}
