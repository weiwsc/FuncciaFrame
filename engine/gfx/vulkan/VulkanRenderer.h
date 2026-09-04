//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#include <filesystem>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "GraphicsPipeline.h"
#include "SlangShaderCompiler.h"
#include "VulkanDevice.h"
#include "VulkanFrame.h"
#include "VulkanInclude.h"
#include "VulkanInstance.h"
#include "VulkanSwapChain.h"
#include "VulkanUploadContext.h"
#include "platform/WindowInterface.h"
#include "resource_management/DescriptorTypeDef.h"
#include "resource_management/FrameSceneDataDescriptorSet.h"
#include "resource_management/TextureSamplerDescriptorSet.h"
#include "types/Model.h"

namespace vva::gfx::vulkan{
    struct FrameContext {
        glm::ivec2 framebufferSize {0, 0};
        glm::vec4 clearColor {1.0f, 1.0f, 1.0f, 1.0f};
    };

    struct VulkanRendererDesc {
        std::string app_name{"FuncciaFrame"};
        std::filesystem::path shader_dir;     // directory Slang searches for modules
        bool enable_validation{true};
    };

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
        std::vector<vk::raii::Sampler> samplers;
        Texture2D depth_resource;
        Model models;
    };


    class VulkanRenderer {
    public:
        explicit VulkanRenderer(VulkanContext context, GlobalDescriptors descriptors);

        static auto createVulkanRenderer(WindowInterface& window_interface,
                                         const VulkanRendererDesc& desc) -> VulkanRenderer;
        auto loadModel(ModelLoadInfo model_load_info) -> ModelHandle;
        auto drawModel(ModelHandle model_handle) -> void;
    private:
        static auto createSurface(WindowInterface& window, const vk::raii::Instance& instance) -> vk::raii::SurfaceKHR;
        VulkanContext context_;
        GlobalDescriptors descriptors_;
    };
}
