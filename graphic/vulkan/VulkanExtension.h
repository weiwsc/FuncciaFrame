//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "VulkanInclude.h"
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

namespace Funccia::Graphic::Vulkan {
    class VulkanExtension {
    public:
        static auto GetRequiredExtensions(const vk::raii::Context& context,
                                          const bool enableValidationLayers) -> std::vector<char const*> {
            uint32_t sdlExtensionCount = 0;
            auto sdlExtensions = SDL_Vulkan_GetInstanceExtensions(&sdlExtensionCount);
            std::vector requiredExtensions(sdlExtensions, sdlExtensions + sdlExtensionCount);

            if (enableValidationLayers) {
                requiredExtensions.push_back(vk::EXTDebugUtilsExtensionName);
            }

            // Check if the required extensions are supported by the Vulkan implementation.
            auto extensionProperties = context.enumerateInstanceExtensionProperties();
            for (auto const& requiredExtension : requiredExtensions) {
                if (std::ranges::none_of(extensionProperties,
                                         [requiredExtension](auto const& extensionProperty) {
                                             return strcmp(extensionProperty.extensionName, requiredExtension) == 0;
                                         })) {
                    throw std::runtime_error("Required extension not supported: " + std::string(requiredExtension));
                }
            }
            return requiredExtensions;
        }
    };
}
