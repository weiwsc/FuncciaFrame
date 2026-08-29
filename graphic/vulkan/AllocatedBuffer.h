//
// Created by Wangsicong Wei on 2026-08-29.
//

#pragma once
#include "VulkanInclude.h"
#include "vk_mem_alloc.h"

namespace Funccia::Graphic::Vulkan {
    struct AllocatedBuffer {
        AllocatedBuffer(VmaAllocator allocator, vk::Buffer buffer, VmaAllocation allocation);

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
    
    private:
        void reset() noexcept;
        VmaAllocator allocator_ = VK_NULL_HANDLE;
        vk::Buffer buffer_;
        VmaAllocation allocation_ = VK_NULL_HANDLE;
    };
}
