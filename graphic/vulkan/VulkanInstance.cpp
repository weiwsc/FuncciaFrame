//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanInstance.h"

#include "../../App.h"

namespace Funccia::Graphic::Vulkan {
    void VulkanInstance::Init() {
        vk::ApplicationInfo application_info {
            .pApplicationName = Funccia::App::Instance().application_metadata.applicationTitle.data(),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Funccia Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14
        };
    }
}
