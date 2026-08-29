//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "VulkanDevice.h"
#include "VulkanFrame.h"
#include "../GraphicsDevice.h"
#include "VulkanInclude.h"
#include "VulkanInstance.h"
#include "VulkanSwapChain.h"
#include "VulkanUploadContext.h"
#include "../shaders/SlangShaderCompiler.h"

namespace Funccia::Graphic::Vulkan {
    struct VulkanContext {
        VulkanInstance instance;
        VulkanDevice device;
        VulkanAllocator allocator;
        vk::raii::SurfaceKHR surface{nullptr};
        VulkanSwapChain swap_chain;
        VulkanFrameController frame_controller;
        VulkanUploadContext upload_context;
        Graphics::Shader::SlangShaderCompiler slang_shader_compiler;
    };
    class VulkanRenderer : public GraphicsDevice {
    public:
        VulkanRenderer(VulkanContext context);
        void Init(WindowInterface& window) override;

        void BeginFrame(const FrameContext& frameContext) override {
        };

        static auto CreateVulkanRenderer(WindowInterface& window_interface) -> VulkanRenderer;

    private:
        static auto CreateSurface(WindowInterface& window, const vk::raii::Instance& instance) -> vk::raii::SurfaceKHR;
        VulkanContext context_;
    };
}
