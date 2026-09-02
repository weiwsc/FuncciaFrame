//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include <iostream>
#include <ostream>

#include "VulkanInclude.h"
#include "../WindowInterface.h"
#include "../../core/Singleton.h"

namespace vva::gfx::vulkan{
    struct VulkanInstance {
        vk::raii::Context context;
        vk::raii::Instance instance = nullptr;
        bool enable_validation_layers = true;
        vk::raii::DebugUtilsMessengerEXT debug_messenger = nullptr;

        static auto createVulkanInstance(bool enable_validation_layers) -> VulkanInstance;
    };
}
