//
// Created by Wangsicong Wei on 2026-08-27.
//
#pragma once

#include "VulkanInclude.h"

namespace vva::gfx::vulkan {
    struct VulkanSetupConfig {
        static constexpr bool ENABLE_VALIDATION_LAYERS = false;
    };
    struct VulkanRenderConfig {
        static constexpr uint32_t MAX_FRAME_IN_FLIGHT = 2;
        static constexpr uint32_t MAX_BINDLESS_TEXTURE = 1280;
        static constexpr uint32_t MAX_SAMPLERS = 6;
    };

    struct VulkanDeviceRequirement {
        static constexpr uint32_t MINIMUM_API_VERSION = vk::ApiVersion14;
        static constexpr std::array EXTENSIONS{
            vk::KHRSwapchainExtensionName,
            //vk::KHRPortabilityEnumerationExtensionName
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

            auto& device_features = chain.get<vk::PhysicalDeviceFeatures2>();
            device_features.features.samplerAnisotropy = vk::True;
            device_features.features.shaderInt64 = vk::True;

            chain.get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters = vk::True;
            auto& vk12 = chain.get<vk::PhysicalDeviceVulkan12Features>();
            vk12.bufferDeviceAddress = vk::True;
            //vk12.runtimeDescriptorArray = vk::True; //for Texture[] that can vary in size
            vk12.shaderSampledImageArrayNonUniformIndexing = vk::True;
            //for using textures[material.textureIndex] in the shader
            vk12.descriptorBindingPartiallyBound = vk::True;
            vk12.descriptorBindingUpdateUnusedWhilePending = vk::True;
            vk12.descriptorBindingSampledImageUpdateAfterBind = vk::True;
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
                ([&supported]() -> bool {
                    const auto& device = supported.get<vk::PhysicalDeviceFeatures2>();
                    return device.features.samplerAnisotropy &&
                        device.features.shaderInt64;
                }()) &&
                supported.get<vk::PhysicalDeviceVulkan11Features>()
                         .shaderDrawParameters &&
                ([&supported]() -> bool {
                    const auto& v12 = supported.get<vk::PhysicalDeviceVulkan12Features>();
                    return v12.bufferDeviceAddress &&
                        v12.descriptorBindingPartiallyBound &&
                        v12.descriptorBindingUpdateUnusedWhilePending &&
                        v12.shaderSampledImageArrayNonUniformIndexing &&
                        v12.descriptorBindingSampledImageUpdateAfterBind;
                }()) &&
                supported.get<vk::PhysicalDeviceVulkan13Features>()
                         .dynamicRendering &&
                supported.get<vk::PhysicalDeviceVulkan13Features>()
                         .synchronization2 &&
                supported.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>()
                         .extendedDynamicState;
        }
    };
}
