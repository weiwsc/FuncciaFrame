//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanInstance.h"

#include "util/VulkanInstanceExtensionUtil.h"
#include "util/VulkanLayerUtil.h"
#include "util/VulkanPhysicalDeviceUtil.h"
#include "../../App.h"
#include "util/VulkanDebugMessengerUtil.h"

namespace Funccia::Graphic::Vulkan {


    void VulkanInstance::createInstance() {
        vk::ApplicationInfo application_info{
            .pApplicationName = Funccia::App::Instance().application_metadata.applicationTitle.data(),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Funccia Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14
        };

        auto required_layers = Util::GetRequiredLayers(context, enableValidationLayers);
        auto required_extensions = Util::GetRequiredInstanceExtensions(context, enableValidationLayers);

        vk::InstanceCreateInfo create_info{
            .pApplicationInfo = &application_info,
            .enabledLayerCount = static_cast<uint32_t>(required_layers.size()),
            .ppEnabledLayerNames = required_layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(required_extensions.size()),
            .ppEnabledExtensionNames = required_extensions.data()
        };
        instance = vk::raii::Instance(context, create_info);
    }

    void VulkanInstance::Init() {
        createInstance();
        if (auto result = Util::CreateDebugMessenger(instance, enableValidationLayers)) {
            debugMessenger = std::move(*result);
        } else {
            //TODO: handle if debug messenger is enabled but failed to setup
        }


    }




}
