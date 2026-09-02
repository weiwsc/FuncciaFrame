//
// Created by Wangsicong Wei on 2026-08-27.
//
#pragma once

#include "VulkanInclude.h"

namespace vva::gfx::vulkan{
    struct VulkanRenderConfig {
        static constexpr uint32_t max_frame_in_flight = 2;
    };

    struct VulkanDeviceRequirement {
        static constexpr uint32_t minimum_api_version = vk::ApiVersion14;
        static constexpr std::array extensions{
            vk::KHRSwapchainExtensionName
        };

        static constexpr vk::QueueFlags required_queue_flags =
            vk::QueueFlagBits::eGraphics |
            vk::QueueFlagBits::eCompute |
            vk::QueueFlagBits::eTransfer;

        using VulkanFeatureChain = vk::StructureChain<
            vk::PhysicalDeviceFeatures2,
            vk::PhysicalDeviceVulkan11Features,
            vk::PhysicalDeviceVulkan12Features,
            vk::PhysicalDeviceVulkan13Features,
            vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>;

        [[nodiscard]]
        static auto features() -> VulkanFeatureChain {
            VulkanFeatureChain chain;

            chain.get<vk::PhysicalDeviceFeatures2>().features.samplerAnisotropy = vk::True;
            chain.get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters = vk::True;
            chain.get<vk::PhysicalDeviceVulkan12Features>().bufferDeviceAddress = vk::True;
            chain.get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering = vk::True;
            chain.get<vk::PhysicalDeviceVulkan13Features>().synchronization2 = vk::True;
            chain.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState = vk::True;

            return chain;
        }

        [[nodiscard]]
        static auto isRequiredFeatureSupported(const vk::raii::PhysicalDevice& device) -> bool {
            auto supported = device.getFeatures2<
                vk::PhysicalDeviceFeatures2,
                vk::PhysicalDeviceVulkan11Features,
                vk::PhysicalDeviceVulkan12Features,
                vk::PhysicalDeviceVulkan13Features,
                vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

            return
                supported.get<vk::PhysicalDeviceFeatures2>()
                         .features.samplerAnisotropy &&
                supported.get<vk::PhysicalDeviceVulkan11Features>()
                         .shaderDrawParameters &&
                supported.get<vk::PhysicalDeviceVulkan12Features>()
                         .bufferDeviceAddress &&
                supported.get<vk::PhysicalDeviceVulkan13Features>()
                         .dynamicRendering &&
                supported.get<vk::PhysicalDeviceVulkan13Features>()
                         .synchronization2 &&
                supported.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>()
                         .extendedDynamicState;
        }
    };
}
