//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "VulkanDevice.h"
#include "../GraphicsDevice.h"
#include "VulkanInclude.h"
#include "VulkanInstance.h"
#include "VulkanSwapChain.h"

namespace Funccia::Graphic::Vulkan {
    class VulkanRenderer : public GraphicsDevice {
    public:
        VulkanRenderer(VulkanInstance, VulkanDevice, vk::raii::SurfaceKHR, VulkanSwapChain);
        void Init(WindowInterface& window) override;

        void BeginFrame(const FrameContext& frameContext) override {
        };

        static auto CreateVulkanRenderer(WindowInterface& window_interface) -> VulkanRenderer;

    private:
        static auto CreateSurface(WindowInterface& window, const vk::raii::Instance& instance) -> vk::raii::SurfaceKHR;

        VulkanInstance instance{};
        VulkanDevice device{};
        vk::raii::SurfaceKHR surface{nullptr};
        VulkanSwapChain swap_chain{};
    };
}
