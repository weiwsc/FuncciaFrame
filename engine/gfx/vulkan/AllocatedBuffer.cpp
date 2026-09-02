//
// Created by Wangsicong Wei on 2026-08-29.
//

#include "AllocatedBuffer.h"

namespace vva::gfx::vulkan{
    AllocatedBuffer::AllocatedBuffer(VmaAllocator allocator, vk::Buffer buffer, VmaAllocation allocation,
                                     vk::DeviceSize size, void* mapped_data) :
        allocator_(allocator), buffer_(buffer), allocation_(allocation),
        size_(size), mapped_data_(mapped_data) {
    }

    AllocatedBuffer::AllocatedBuffer(AllocatedBuffer&& other) noexcept :
        allocator_(std::exchange(other.allocator_, VK_NULL_HANDLE)),
        buffer_(std::exchange(other.buffer_, nullptr)),
        allocation_(std::exchange(other.allocation_, VK_NULL_HANDLE)),
        size_(other.size_),
        mapped_data_(other.mapped_data_) {
    }

    auto AllocatedBuffer::operator=(AllocatedBuffer&& other) noexcept -> AllocatedBuffer& {
        if (this != &other) {
            reset();
            allocator_ = std::exchange(other.allocator_, VK_NULL_HANDLE);
            buffer_ = std::exchange(other.buffer_, nullptr);
            allocation_ = std::exchange(other.allocation_, VK_NULL_HANDLE);
            size_ = std::exchange(other.size_, 0);
            mapped_data_ = std::exchange(other.mapped_data_, nullptr);
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

    auto AllocatedBuffer::createBuffer(VmaAllocator allocator, vk::DeviceSize size, vk::BufferUsageFlags usage_flags,
                                       VmaMemoryUsage memory_usage,
                                       VmaAllocationCreateFlags allocation_create_flags) -> AllocatedBuffer {
        vk::BufferCreateInfo buffer_create_info {
            .size = size,
            .usage = usage_flags,
            .sharingMode = vk::SharingMode::eExclusive
        };
        VmaAllocationCreateInfo allocation_create_info {
            .flags = allocation_create_flags,
            .usage = memory_usage
        };
        VkBuffer raw_buffer = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
        VmaAllocationInfo allocation_info{};

        const VkResult result = vmaCreateBuffer(
            allocator,
            &*buffer_create_info,
            &allocation_create_info,
            &raw_buffer,
            &allocation,
            &allocation_info
        );
        if (result != VK_SUCCESS) {
            throw std::runtime_error(
                "vmaCreateBuffer failed: " +
                vk::to_string(static_cast<vk::Result>(result)));
        }
        return AllocatedBuffer{
            allocator,
            vk::Buffer{raw_buffer},
            allocation,
            size,
            allocation_info.pMappedData
        };
    }
}
