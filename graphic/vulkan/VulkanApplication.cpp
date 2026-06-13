//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanApplication.h"

namespace Funccia::Graphic::Vulkan{
    VulkanApplication::VulkanApplication() {
    }

    void VulkanApplication::Init(WindowInterface& window) {
        vk_instance_.Init(window);
    }
}