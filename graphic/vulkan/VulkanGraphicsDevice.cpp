//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanGraphicsDevice.h"

#include <SDL3/SDL_vulkan.h>

#include "util/VulkanLogicalDeviceUtil.h"
#include "util/VulkanPhysicalDeviceUtil.h"

namespace Funccia::Graphic::Vulkan {
    VulkanGraphicsDevice::VulkanGraphicsDevice() = default;

    void VulkanGraphicsDevice::Init(WindowInterface& window) {
        instance.Init();
        surface = CreateSurface(window, instance.instance);
        auto physical_device = Util::pickPhysicalDevice(instance.instance, VulkanDevice::requiredDeviceExtensions);
        device = Util::CreateVulkanDevice(surface, physical_device);
    }

    auto VulkanGraphicsDevice::CreateSurface(WindowInterface& window,
                                                             const vk::raii::Instance& instance)->vk::raii::SurfaceKHR {
        VkSurfaceKHR _surface;
        if (!SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(window.GetNativeWindow()), *instance, nullptr,
                                      &_surface)) {
            throw std::runtime_error("failed to create window surface!");
                                      }
        return vk::raii::SurfaceKHR(instance, _surface);
    }
}
