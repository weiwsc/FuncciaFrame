//
// Created by Wangsicong Wei on 2026-08-29.
//

#include "VulkanAllocator.h"

#include <vk_mem_alloc.h>

#include "VulkanConfig.h"
#include "VulkanDevice.h"
#include "core/Log.h"

namespace vva::gfx::vulkan{
    auto VulkanAllocator::create(
        const vk::raii::PhysicalDevice& physical_device,
        const vk::raii::Device& device,
        const vk::raii::Instance& instance) -> VulkanAllocator {
        VmaAllocatorCreateInfo create_info{
            .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
            .physicalDevice = static_cast<VkPhysicalDevice>(*physical_device),
            .device = static_cast<VkDevice>(*device),
            .instance = static_cast<VkInstance>(*instance),
            .vulkanApiVersion = VulkanDeviceRequirement::minimum_api_version
        };
        VmaAllocator handle{};
        const VkResult result = vmaCreateAllocator(&create_info, &handle);

        if (result != VK_SUCCESS) {
            throw std::runtime_error(
                "failed to create VMA allocator: " +
                vk::to_string(static_cast<vk::Result>(result)));
        }
        vva_log_info("allocator created.");
        return {handle};
    }

    auto VulkanAllocator::operator=(VulkanAllocator&& other) noexcept -> VulkanAllocator& {
        if (this != &other) {
            reset();
            handle_ = std::exchange(other.handle_, nullptr);
        }
        return *this;
    }

    auto VulkanAllocator::reset() noexcept -> void {
        if (handle_) {
            vmaDestroyAllocator(handle_);
            handle_ = nullptr;
        }
    }

    VulkanAllocator::VulkanAllocator(VmaAllocator handle) : handle_(handle) {
    }
}
