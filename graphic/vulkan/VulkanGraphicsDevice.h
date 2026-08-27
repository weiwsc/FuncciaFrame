//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "VulkanDevice.h"
#include "../GraphicsDevice.h"
#include "VulkanInclude.h"
#include "VulkanInstance.h"

namespace Funccia::Graphic::Vulkan {
    class VulkanGraphicsDevice : public GraphicsDevice {
    public:
        VulkanGraphicsDevice();
        void Init(WindowInterface& window) override;

        void BeginFrame(const FrameContext& frameContext) override {
        };

    private:
        static auto CreateSurface(WindowInterface& window, const vk::raii::Instance& instance) -> vk::raii::SurfaceKHR;

        VulkanInstance instance{};
        VulkanDevice device{};
        vk::raii::SurfaceKHR surface{nullptr};
    };
}
