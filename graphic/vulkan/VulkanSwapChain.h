//
// Created by Wangsicong Wei on 2026-08-27.
//

#pragma once
#include <glm/vec2.hpp>

#include "VulkanImage.h"
#include "VulkanInclude.h"

namespace Funccia::Graphic::Vulkan {
    struct VulkanSwapChain {
        vk::raii::SwapchainKHR handle{nullptr};
        std::vector<VulkanSwapChainImage> images;
        vk::SurfaceFormatKHR surface_format{};
        vk::Extent2D extent{};

        static auto CreateSwapChain(const vk::raii::SurfaceKHR& surface,
                                          const vk::raii::PhysicalDevice& physical_device,
                                          const vk::raii::Device& device,
                                          const glm::ivec2 buffer_size) -> VulkanSwapChain;
    };
}
