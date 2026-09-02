//
// Created by Wangsicong Wei on 2026-07-27.
//

#pragma once
#include <optional>
#include "../VulkanInclude.h"

namespace vva::gfx::vulkan::util {
    [[nodiscard]]
    auto createDebugMessenger(const vk::raii::Instance& instance,
                              bool enable_validation_layers) -> std::optional<vk::raii::DebugUtilsMessengerEXT>;
}
