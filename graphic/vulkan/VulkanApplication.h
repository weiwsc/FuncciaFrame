//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "../GraphicsDevice.h"
#include "VulkanInclude.h"
#include "VulkanContext.h"

namespace Funccia::Graphic::Vulkan {
    class VulkanApplication : public GraphicsDevice {
    public:
        VulkanApplication();
        void Init(WindowInterface& window) override;

        void BeginFrame(const FrameContext& frameContext) override {
        };

    private:
        VulkanContext vk_instance_{};
    };
}
