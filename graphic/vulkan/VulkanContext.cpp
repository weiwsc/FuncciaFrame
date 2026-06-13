//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanContext.h"

#include "VulkanExtension.h"
#include "VulkanLayer.h"
#include "VulkanPhysicalDevice.h"
#include "../../App.h"

namespace Funccia::Graphic::Vulkan {
    static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                  vk::DebugUtilsMessageTypeFlagsEXT type,
                                                  const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                  void *) {
        std::cerr << "validation layer: type " << to_string(type) << " msg: " << pCallbackData->pMessage << std::endl;

        return vk::False;
    }

    void VulkanContext::createInstance() {
        vk::ApplicationInfo application_info {
            .pApplicationName = Funccia::App::Instance().application_metadata.applicationTitle.data(),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Funccia Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14
        };

        auto required_layers = VulkanLayer::GetRequiredLayers(context, enableValidationLayers);
        auto required_extensions = VulkanExtension::GetRequiredExtensions(context, enableValidationLayers);

        vk::InstanceCreateInfo create_info{
            .pApplicationInfo = &application_info,
            .enabledLayerCount = static_cast<uint32_t>(required_layers.size()),
            .ppEnabledLayerNames = required_layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(required_extensions.size()),
            .ppEnabledExtensionNames = required_extensions.data()
        };
        instance_ = vk::raii::Instance(context, create_info);
    }

    void VulkanContext::Init(WindowInterface& window) {
        createInstance();
        setupDebugMessenger();
        setupSurface(window);
        physical_device = VulkanPhysicalDevice::pickPhysicalDevice(instance_, requiredDeviceExtensions);
    }

    void VulkanContext::setupDebugMessenger() {
        if (!enableValidationLayers) return;
        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
        vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
            .messageSeverity = severityFlags,
            .messageType = messageTypeFlags,
            .pfnUserCallback = &debugCallback
        };
        debugMessenger = instance_.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
    }

    void VulkanContext::setupSurface(WindowInterface& window) {
        VkSurfaceKHR _surface;
        if (!SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(window.GetNativeWindow()), *instance_, nullptr, &_surface)) {
            throw std::runtime_error("failed to create window surface!");
        }
        surface = vk::raii::SurfaceKHR(instance_, _surface);
    }
}
