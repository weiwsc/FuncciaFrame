//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "VulkanInclude.h"
#include <algorithm>
#include <vector>

namespace Funccia::Graphic::Vulkan {
    class VulkanLayer {
    public:
        static auto GetRequiredLayers(const vk::raii::Context& context, const bool enableValidationLayers) -> std::vector<char const*>  {
            std::vector<char const *> requiredLayers;

            const std::vector<char const *> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
            };
            if (enableValidationLayers) {
                requiredLayers.assign(validationLayers.begin(), validationLayers.end());
            }

            // Check if the required layers are supported by the Vulkan implementation.
            auto layerProperties = context.enumerateInstanceLayerProperties();
            if (std::ranges::any_of(requiredLayers, [&layerProperties](auto const &requiredLayer) {
                return std::ranges::none_of(layerProperties,
                                            [requiredLayer](auto const &layerProperty) {
                                                return strcmp(layerProperty.layerName, requiredLayer) == 0;
                                            });
            })) {
                throw std::runtime_error("One or more required layers are not supported!");
            }
            return requiredLayers;
        }
    };
}
