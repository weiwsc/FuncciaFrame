//
// Created by Wangsicong Wei on 2026-07-27.
//

#pragma once
#include <optional>
#include "../VulkanInclude.h"

namespace Funccia::Graphic::Vulkan::Util {
    [[nodiscard]]
    auto CreateDebugMessenger(const vk::raii::Instance& instance,
                              bool enableValidationLayers) -> std::optional<vk::raii::DebugUtilsMessengerEXT>;
}
