//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include <iostream>
#include <ostream>

#include "VulkanInclude.h"
#include "../WindowInterface.h"
#include "../../core/Singleton.h"

namespace Funccia::Graphic::Vulkan {
    struct VulkanContext {
        void Init(WindowInterface& window);

        vk::raii::Instance instance_ = nullptr;
        vk::raii::Context context;
        bool enableValidationLayers = true;
        vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;
        vk::raii::SurfaceKHR surface = nullptr;
        vk::raii::PhysicalDevice physical_device = nullptr;

    private:
        void createInstance();
        void setupDebugMessenger();
        void setupSurface(WindowInterface& window);

        static constexpr std::array requiredDeviceExtensions = {
            vk::KHRSwapchainExtensionName,
        };
    };
}
