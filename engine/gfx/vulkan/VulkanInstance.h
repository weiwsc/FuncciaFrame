//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include <string>

#include "VulkanInclude.h"

namespace vva::gfx::vulkan{
    struct VulkanInstance {
        vk::raii::Context context;
        vk::raii::Instance instance = nullptr;
        bool enable_validation_layers = true;
        vk::raii::DebugUtilsMessengerEXT debug_messenger = nullptr;

        static auto createVulkanInstance(const std::string& app_name, bool enable_validation_layers) -> VulkanInstance;
    };
}
