//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanInstance.h"

#include "VulkanConfig.h"
#include "core/Log.h"
#include "util/VulkanDebugMessengerUtil.h"
#include "util/VulkanInstanceExtensionUtil.h"
#include "util/VulkanLayerUtil.h"

namespace vva::gfx::vulkan{
    auto VulkanInstance::createVulkanInstance(const std::string& app_name,
                                              const bool enable_validation_layers) -> VulkanInstance {
        vk::ApplicationInfo application_info{
            .pApplicationName = app_name.c_str(),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Funccia Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VulkanDeviceRequirement::minimum_api_version
        };
        vk::raii::Context context;
        auto required_layers = util::getRequiredLayers(context, enable_validation_layers);
        auto required_extensions = util::getRequiredInstanceExtensions(context, enable_validation_layers);

        vk::InstanceCreateInfo create_info{
            .pApplicationInfo = &application_info,
            .enabledLayerCount = static_cast<uint32_t>(required_layers.size()),
            .ppEnabledLayerNames = required_layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(required_extensions.size()),
            .ppEnabledExtensionNames = required_extensions.data()
        };

        auto instance = vk::raii::Instance(context, create_info);
        vva_log_info("vulkan instance created");

        // createDebugMessenger returns nullopt when validation is disabled; that is not a failure.
        auto debug_messenger = util::createDebugMessenger(instance, enable_validation_layers);
        if (enable_validation_layers && !debug_messenger) {
            throw std::runtime_error("validation layers requested but the debug messenger could not be created");
        }

        return VulkanInstance{
            .context = std::move(context),
            .instance = std::move(instance),
            .enable_validation_layers = enable_validation_layers,
            .debug_messenger = debug_messenger ? std::move(*debug_messenger) : vk::raii::DebugUtilsMessengerEXT{nullptr}
        };
    }
}
