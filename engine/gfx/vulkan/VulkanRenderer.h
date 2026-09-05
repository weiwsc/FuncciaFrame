//
// Created by Wangsicong Wei on 2026-06-11.
//

#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <filesystem>
#include <optional>
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
#include "types/Camera.h"
#include "types/Model.h"

namespace vva::gfx::vulkan{
    struct FrameContext {
        glm::ivec2 framebufferSize {0, 0};
        glm::vec4 clearColor {1.0f, 1.0f, 1.0f, 1.0f};
    };

    struct VulkanRendererDesc {
        std::string app_name{"FuncciaFrame"};
        std::filesystem::path shader_dir;     // directory Slang searches for modules
        bool enable_validation{false};
        bool per_frame_depth{true};
        std::optional<vk::PresentModeKHR> present_mode;
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
        // Indexed by frame slot; a slot's fence protects attachment reuse.
        std::vector<Texture2D> depth_resources;
        Model models;
        Camera camera;
    };

    struct FrameStateStore {
        uint32_t image_index;
        uint32_t frame_index;
    };


    class VulkanRenderer {
    public:
        explicit VulkanRenderer(VulkanContext context, GlobalDescriptors descriptors);
        ~VulkanRenderer() {context_.device.logical_device.waitIdle();}
        static auto createVulkanRenderer(WindowInterface& window_interface,
                                         const VulkanRendererDesc& desc) -> VulkanRenderer;
        auto loadModel(ModelLoadInfo model_load_info) -> ModelHandle;
        auto getModelTransform(const ModelHandle handle) -> Transform& {return context_.models.transforms[handle.handle];}
        auto startFrame() -> void;
        auto endFrame() -> void;
        // Drain submitted work for benchmark completion or teardown, not per frame.
        auto waitIdle() -> void { context_.device.logical_device.waitIdle(); }
        auto drawModel(ModelHandle model_handle) -> void;
    private:
        static auto createSurface(WindowInterface& window, const vk::raii::Instance& instance) -> vk::raii::SurfaceKHR;
        auto updateFrameData() -> shader::param::FrameUniformBuffer;
        VulkanContext context_;
        GlobalDescriptors descriptors_;
        float last_time_ {0};
        FrameStateStore frame_state_store_;
    };
}
