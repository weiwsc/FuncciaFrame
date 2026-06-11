//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "../GraphicsDevice.h"
#include "VulkanInclude.h"
namespace Funccia::Graphic::Vulkan {
    class VulkanGraphicsDevice : public GraphicsDevice {
    public:
        VulkanGraphicsDevice() = default;
        void Init(WindowInterface& window) override;

    private:

    };
}
