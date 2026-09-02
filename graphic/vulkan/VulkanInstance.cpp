//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanInstance.h"

#include "VulkanConfig.h"
#include "util/VulkanInstanceExtensionUtil.h"
#include "util/VulkanLayerUtil.h"
#include "util/VulkanPhysicalDeviceUtil.h"
#include "../../App.h"
#include "../../util/Log.h"
#include "util/VulkanDebugMessengerUtil.h"

namespace vva::gfx::vulkan{
    auto VulkanInstance::createVulkanInstance(const bool enable_validation_layers) -> VulkanInstance {
        vk::ApplicationInfo application_info{
            .pApplicationName = vva::App::Instance().application_metadata.applicationTitle.data(),
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

        if (auto result = util::createDebugMessenger(instance, enable_validation_layers)) {
            auto debug_messenger = std::move(*result);

            return VulkanInstance{
                .context = std::move(context),
                .instance = std::move(instance),
                .enable_validation_layers = enable_validation_layers,
                .debug_messenger = std::move(debug_messenger)
            };
        }
        else {
            //TODO: handle if debug messenger is enabled but failed to setup
            vva_log_error("failed to create debug messenger");
            std::abort();
        }
    }
}
