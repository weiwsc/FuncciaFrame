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
    struct VulkanInstance {
        vk::raii::Context context;
        vk::raii::Instance instance = nullptr;
        bool enableValidationLayers = true;
        vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;

        static auto createVulkanInstance(bool enableValidationLayers) -> VulkanInstance;
    };
}
