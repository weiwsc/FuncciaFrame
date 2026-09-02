//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanInstanceExtensionUtil.h"

namespace vva::gfx::vulkan::util {
    auto getRequiredInstanceExtensions(const vk::raii::Context& context,
                                       const bool enable_validation_layers) -> std::vector<char const*> {
        uint32_t sdl_extension_count = 0;
        auto sdl_extensions = SDL_Vulkan_GetInstanceExtensions(&sdl_extension_count);
        std::vector required_extensions(sdl_extensions, sdl_extensions + sdl_extension_count);

        if (enable_validation_layers) {
            required_extensions.push_back(vk::EXTDebugUtilsExtensionName);
        }

        // Check if the required extensions are supported by the Vulkan implementation.
        auto extension_properties = context.enumerateInstanceExtensionProperties();
        for (auto const& required_extension : required_extensions) {
            if (std::ranges::none_of(extension_properties,
                                     [required_extension](auto const& extension_property) {
                                         return strcmp(extension_property.extensionName, required_extension) == 0;
                                     })) {
                throw std::runtime_error("Required extension not supported: " + std::string(required_extension));
            }
        }
        return required_extensions;
    }
}
