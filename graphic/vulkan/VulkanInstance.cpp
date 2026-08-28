//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanInstance.h"

#include "VulkanDeviceRequirement.h"
#include "util/VulkanInstanceExtensionUtil.h"
#include "util/VulkanLayerUtil.h"
#include "util/VulkanPhysicalDeviceUtil.h"
#include "../../App.h"
#include "../../util/Log.h"
#include "util/VulkanDebugMessengerUtil.h"

namespace Funccia::Graphic::Vulkan {
    auto VulkanInstance::createVulkanInstance(const bool enableValidationLayers) -> VulkanInstance {
        vk::ApplicationInfo application_info{
            .pApplicationName = Funccia::App::Instance().application_metadata.applicationTitle.data(),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Funccia Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VulkanDeviceRequirement::minimum_api_version
        };
        vk::raii::Context context;
        auto required_layers = Util::GetRequiredLayers(context, enableValidationLayers);
        auto required_extensions = Util::GetRequiredInstanceExtensions(context, enableValidationLayers);

        vk::InstanceCreateInfo create_info{
            .pApplicationInfo = &application_info,
            .enabledLayerCount = static_cast<uint32_t>(required_layers.size()),
            .ppEnabledLayerNames = required_layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(required_extensions.size()),
            .ppEnabledExtensionNames = required_extensions.data()
        };

        auto instance = vk::raii::Instance(context, create_info);
        vva_log_info("vulkan instance created");

        if (auto result = Util::CreateDebugMessenger(instance, enableValidationLayers)) {
            auto debugMessenger = std::move(*result);

            return VulkanInstance{
                .context = std::move(context),
                .instance = std::move(instance),
                .enableValidationLayers = enableValidationLayers,
                .debugMessenger = std::move(debugMessenger)
            };
        }
        else {
            //TODO: handle if debug messenger is enabled but failed to setup
            vva_log_error("failed to create debug messenger");
            std::abort();
        }
    }
}
