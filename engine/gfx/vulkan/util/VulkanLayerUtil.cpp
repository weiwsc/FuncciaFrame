//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanLayerUtil.h"

#include "gfx/vulkan/VulkanConfig.h"

namespace vva::gfx::vulkan::util {
    auto getRequiredLayers(const vk::raii::Context& context,
                           const bool enable_validation_layers) -> std::vector<char const*> {
        std::vector<char const*> required_layers;

        const std::vector<char const*> validation_layers = {
           "VK_LAYER_KHRONOS_validation"
        };
        if (VulkanSetupConfig::ENABLE_VALIDATION_LAYERS) {
            required_layers.assign(validation_layers.begin(), validation_layers.end());
        }

        // Check if the required layers are supported by the Vulkan implementation.
        auto layer_properties = context.enumerateInstanceLayerProperties();
        if (std::ranges::any_of(required_layers, [&layer_properties](auto const& required_layer) {
            return std::ranges::none_of(layer_properties,
                                        [required_layer](auto const& layer_property) {
                                            return strcmp(layer_property.layerName, required_layer) == 0;
                                        });
        })) {
            throw std::runtime_error("One or more required layers are not supported!");
        }
        return required_layers;
    }
}
