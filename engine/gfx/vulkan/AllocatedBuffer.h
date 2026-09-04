//
// Created by Wangsicong Wei on 2026-08-29.
//

#pragma once
#include "VulkanInclude.h"
#include "vk_mem_alloc.h"

namespace vva::gfx::vulkan{
    struct AllocatedBuffer {
        AllocatedBuffer(VmaAllocator allocator, vk::Buffer buffer, VmaAllocation allocation,
            vk::DeviceSize size, void* mapped_data = nullptr);

        AllocatedBuffer(const AllocatedBuffer&) = delete;
        auto operator=(const AllocatedBuffer&) -> AllocatedBuffer& = delete;

        AllocatedBuffer(AllocatedBuffer&& other) noexcept;

        auto operator=(AllocatedBuffer&& other) noexcept -> AllocatedBuffer& ;

        ~AllocatedBuffer() { reset(); }

        [[nodiscard]]
        auto handle() const noexcept -> vk::Buffer {
            return buffer_;
        }

        [[nodiscard]]
        auto allocation() const noexcept -> VmaAllocation {
            return allocation_;
        }

        [[nodiscard]] auto size() const noexcept -> vk::DeviceSize {
            return size_;
        }

        [[nodiscard]] auto mappedData() const noexcept -> void* {
            return mapped_data_;
        }
        static auto createBuffer(
            VmaAllocator allocator,
            vk::DeviceSize size,
            vk::BufferUsageFlags usage_flags,
            VmaMemoryUsage memory_usage,
            VmaAllocationCreateFlags allocation_create_flags = 0
            ) ->AllocatedBuffer;

        auto flush(vk::DeviceSize offset = 0, vk::DeviceSize size = VK_WHOLE_SIZE) const -> void {
            vmaFlushAllocation(allocator_, allocation_, offset, size);
        }
    private:
        void reset() noexcept;
        VmaAllocator allocator_ = VK_NULL_HANDLE;
        vk::Buffer buffer_;
        VmaAllocation allocation_ = VK_NULL_HANDLE;
        vk::DeviceSize size_ = 0;
        void* mapped_data_ = nullptr;
    };
}
