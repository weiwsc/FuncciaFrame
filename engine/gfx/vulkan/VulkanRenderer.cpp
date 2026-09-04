//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanRenderer.h"

#include <SDL3/SDL_vulkan.h>

#include "core/Log.h"
#include "types/Sampler.h"
#include "types/Texture.h"
#include "util/VulkanLogicalDeviceUtil.h"
#include "util/VulkanPhysicalDeviceUtil.h"

namespace vva::gfx::vulkan {
    VulkanRenderer::VulkanRenderer(VulkanContext context, GlobalDescriptors descriptors)
        : context_(std::move(context)), descriptors_(std::move(descriptors)) {
    }

    auto VulkanRenderer::createVulkanRenderer(WindowInterface& window_interface,
                                              const VulkanRendererDesc& desc) -> VulkanRenderer {
        auto instance = VulkanInstance::createVulkanInstance(desc.app_name, desc.enable_validation);
        auto surface = createSurface(window_interface, instance.instance);
        auto physical_device_selection_result = util::pickPhysicalDevice(instance.instance, surface);
        auto physical_device = physical_device_selection_result.physical_device;
        auto device = util::createVulkanDevice(physical_device, physical_device_selection_result.queue_coordinates);

        auto allocator = VulkanAllocator::create(physical_device, device.logical_device, instance.instance);

        glm::ivec2 buffer_size{};
        window_interface.GetFramebufferSize(buffer_size.x, buffer_size.y);
        auto swap_chain =
            VulkanSwapChain::createSwapChain(surface, physical_device, device.logical_device, buffer_size);
        auto frame_controller = VulkanFrameController::create(device);
        auto upload_context = createVulkanUploadContext(device);
        auto shader_compiler = shader::SlangShaderCompiler::create(desc.shader_dir);

        auto texture_sampler_set = TextureSamplerDescriptorSet::create(device.logical_device);
        auto frame_scene_data_set = FrameSceneDataDescriptorSet::create(device.logical_device, allocator.get());
        GlobalDescriptors global_descriptors = {
            .texture_sampler = std::move(texture_sampler_set),
            .frame_scene_data = std::move(frame_scene_data_set)
        };
        auto graphics_pipeline = GraphicsPipeline::create(device.logical_device, physical_device,
                                                          swap_chain.surface_format, shader_compiler,
                                                          global_descriptors);
        auto samplers = createSamplers(device.logical_device);
        auto depth_resource = createDepthResources(allocator.get(), device.logical_device, physical_device,
                                                   swap_chain.extent);

        global_descriptors.texture_sampler.writeSamplers(samplers);

        vva_log_info("vulkan renderer created");
        return VulkanRenderer{
            {
                .instance = std::move(instance),
                .device = std::move(device),
                .allocator = std::move(allocator),
                .surface = std::move(surface),
                .swap_chain = std::move(swap_chain),
                .frame_controller = std::move(frame_controller),
                .upload_context = std::move(upload_context),
                .slang_shader_compiler = std::move(shader_compiler),
                .graphics_pipeline = std::move(graphics_pipeline),
                .samplers = std::move(samplers),
                .depth_resource = std::move(depth_resource),
                .models = {}
            },
            std::move(global_descriptors)
        };
    }

    auto VulkanRenderer::loadModel(ModelLoadInfo model_load_info) -> ModelHandle {
        auto result = context_.models.createModel(model_load_info,
                                                  context_.allocator.get(),
                                                  context_.device,
                                                  context_.upload_context);
        if (result.has_value()) {
            return result.value();
        } else {
            throw std::runtime_error {result.error()};
        }
    }

    auto VulkanRenderer::createSurface(WindowInterface& window,
                                       const vk::raii::Instance& instance) -> vk::raii::SurfaceKHR {
        VkSurfaceKHR surface;
        if (!SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(window.GetNativeWindow()), *instance, nullptr,
                                      &surface)) {
            throw std::runtime_error("failed to create window surface!");
        }
        return vk::raii::SurfaceKHR(instance, surface);
    }
}
