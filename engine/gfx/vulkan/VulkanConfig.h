//
// Created by Wangsicong Wei on 2026-08-27.
//
#pragma once

#include "VulkanInclude.h"

namespace vva::gfx::vulkan{
    struct VulkanRenderConfig {
        static constexpr uint32_t MAX_FRAME_IN_FLIGHT = 2;
        static constexpr uint32_t MAX_BINDLESS_TEXTURE = 1280;
        static constexpr uint32_t MAX_SAMPLERS = 6;
    };

    struct VulkanDeviceRequirement {
        static constexpr uint32_t MINIMUM_API_VERSION = vk::ApiVersion14;
        static constexpr std::array EXTENSIONS{
            vk::KHRSwapchainExtensionName
        };

        static constexpr vk::QueueFlags REQUIRED_QUEUE_FLAGS =
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
