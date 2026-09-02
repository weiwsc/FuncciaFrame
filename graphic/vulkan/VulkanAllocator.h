//
// Created by Wangsicong Wei on 2026-08-29.
//

#pragma once
#include "VulkanInclude.h"
#include <vk_mem_alloc.h>

namespace vva::gfx::vulkan{
    struct VulkanDevice;

    struct VulkanAllocator {
        static auto create(const vk::raii::PhysicalDevice& physical_device,
                           const vk::raii::Device& device,
                           const vk::raii::Instance& instance) -> VulkanAllocator;

        ~VulkanAllocator() { reset(); }
        VulkanAllocator(const VulkanAllocator&) = delete;
        auto operator=(const VulkanAllocator&) -> VulkanAllocator& = delete;

        VulkanAllocator(VulkanAllocator&& other) noexcept : handle_(std::exchange(other.handle_, nullptr)) {
        }

        auto operator=(VulkanAllocator&& other) noexcept -> VulkanAllocator&;

        [[nodiscard]]
        auto get() const noexcept -> VmaAllocator {
            return handle_;
        }

    private:
        auto reset() noexcept -> void;
        VulkanAllocator(VmaAllocator handle);;
        VmaAllocator handle_{};
    };
}
