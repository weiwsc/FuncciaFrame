//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include "GraphicsPipeline.h"
#include "VulkanDevice.h"
#include "VulkanFrame.h"
#include "../GraphicsDevice.h"
#include "VulkanInclude.h"
#include "VulkanInstance.h"
#include "VulkanSwapChain.h"
#include "VulkanUploadContext.h"
#include "../shaders/SlangShaderCompiler.h"

namespace vva::gfx::vulkan{
    struct VulkanContext {
        VulkanInstance instance;
        VulkanDevice device;
        VulkanAllocator allocator;
        vk::raii::SurfaceKHR surface{nullptr};
        VulkanSwapChain swap_chain;
        VulkanFrameController frame_controller;
        VulkanUploadContext upload_context;
        shader::SlangShaderCompiler slang_shader_compiler;
        GraphicsPipeline graphics_pipeline;
        vk::raii::Sampler sampler;
        Texture2D depth_resource;
    };
    class VulkanRenderer : public GraphicsDevice {
    public:
        VulkanRenderer(VulkanContext context);
        void Init(WindowInterface& window) override;

        void BeginFrame(const FrameContext& frame_context) override {
        };

        static auto createVulkanRenderer(WindowInterface& window_interface) -> VulkanRenderer;

    private:
        static auto createSurface(WindowInterface& window, const vk::raii::Instance& instance) -> vk::raii::SurfaceKHR;
        VulkanContext context_;
    };
}
