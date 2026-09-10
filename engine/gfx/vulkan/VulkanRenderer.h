//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
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
#include "resource_management/VramVector.h"
#include "types/Camera.h"
#include "types/Model.h"

namespace vva::gfx::vulkan {
    struct FrameContext {
        glm::ivec2 framebufferSize{0, 0};
        glm::vec4 clearColor{1.0f, 1.0f, 1.0f, 1.0f};
    };

    struct VulkanRendererDesc {
        std::string app_name{"FuncciaFrame"};
        std::filesystem::path shader_dir; // directory Slang searches for modules
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
        //Camera camera;
        std::vector<VramVector<shader::param::BasicDrawData>> draw_datas;
    };

    struct FrameStateStore {
        uint32_t image_index;
        uint32_t frame_index;
        float last_time;
        float time;
        float delta_time;
    };


    class VulkanRenderer {
    public:
        explicit VulkanRenderer(VulkanContext context, GlobalDescriptors descriptors);
        ~VulkanRenderer() { context_.device.logical_device.waitIdle(); }
        static auto createVulkanRenderer(WindowInterface& window_interface,
                                         const VulkanRendererDesc& desc) -> VulkanRenderer;


        auto updateFrameData(const Camera& camera, glm::vec2 mouse_pos) -> void;
        auto startFrame() -> void;
        auto endFrame() -> void;
        auto drawModel(Model model) -> void;

        auto resourceRegistry() -> GpuResourceRegistry& {
            return gpu_resource_registry_;
        }

    private:
        static auto createSurface(WindowInterface& window, const vk::raii::Instance& instance) -> vk::raii::SurfaceKHR;

        VulkanContext context_;
        GlobalDescriptors descriptors_;
        float last_time_{0};
        FrameStateStore frame_state_store_;
        GpuResourceRegistry gpu_resource_registry_;
    };
}
