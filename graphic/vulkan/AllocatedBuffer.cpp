//
// Created by Wangsicong Wei on 2026-08-29.
//

#include "AllocatedBuffer.h"

namespace Funccia::Graphic::Vulkan {
    AllocatedBuffer::AllocatedBuffer(VmaAllocator allocator, vk::Buffer buffer, VmaAllocation allocation) :
        allocator_(allocator), buffer_(buffer), allocation_(allocation) {
    }

    AllocatedBuffer::AllocatedBuffer(AllocatedBuffer&& other) noexcept :
        allocator_(std::exchange(other.allocator_, VK_NULL_HANDLE)),
        buffer_(std::exchange(other.buffer_, nullptr)),
        allocation_(std::exchange(other.allocation_, VK_NULL_HANDLE)) {
    }

    auto AllocatedBuffer::operator=(AllocatedBuffer&& other) noexcept -> AllocatedBuffer& {
        if (this != &other) {
            reset();
            allocator_ = std::exchange(other.allocator_, VK_NULL_HANDLE);
            buffer_ = std::exchange(other.buffer_, nullptr);
            allocation_ = std::exchange(other.allocation_, VK_NULL_HANDLE);
        }
        return *this;
    }

    auto AllocatedBuffer::reset() noexcept -> void {
        if (allocator_ && buffer_) {
            vmaDestroyBuffer(allocator_, static_cast<VkBuffer>(buffer_), allocation_);
        }
        allocator_ = VK_NULL_HANDLE;
        buffer_ = nullptr;
        allocation_ = VK_NULL_HANDLE;
    }
}
