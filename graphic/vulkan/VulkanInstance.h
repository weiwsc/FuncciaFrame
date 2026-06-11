//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "VulkanInclude.h"
#include "../../core/Singleton.h"

namespace Funccia::Graphic::Vulkan {
    class VulkanInstance : public Core::Singleton<VulkanInstance> {
    public:
        void Init();
    private:
        vk::raii::Instance instance_;
    };
}
