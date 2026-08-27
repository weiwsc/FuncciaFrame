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
        void Init();

        vk::raii::Instance instance = nullptr;
        vk::raii::Context context;
        bool enableValidationLayers = true;
        vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;



    private:
        void createInstance();
    };
}
