//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanRenderer.h"

#include <SDL3/SDL_vulkan.h>

#include "../../util/Log.h"
#include "types/Texture.h"
#include "util/VulkanLogicalDeviceUtil.h"
#include "util/VulkanPhysicalDeviceUtil.h"

namespace vva::gfx::vulkan{
    VulkanRenderer::VulkanRenderer(VulkanContext context)
        : context_(std::move(context)) {
    }

    void VulkanRenderer::Init(WindowInterface& window) {
        // TODO: deprecate init() style intialization in the interface
    }

    auto VulkanRenderer::createVulkanRenderer(WindowInterface& window_interface) -> VulkanRenderer {
        auto instance = VulkanInstance::createVulkanInstance(true);
        auto surface = createSurface(window_interface, instance.instance);
        auto physical_device_selection_result = util::pickPhysicalDevice(instance.instance, surface);
        auto physical_device = physical_device_selection_result.physical_device;
        auto device = util::createVulkanDevice(physical_device, physical_device_selection_result.queue_coordinates);

        auto allocator = VulkanAllocator::create(physical_device, device.logical_device, instance.instance);

        ivec2 buffer_size{};
        window_interface.GetFramebufferSize(buffer_size.x, buffer_size.y);
        auto swap_chain =
            VulkanSwapChain::createSwapChain(surface, physical_device, device.logical_device, buffer_size);
        auto frame_controller = VulkanFrameController::create(device);
        auto upload_context = createVulkanUploadContext(device);
        auto shader_compiler = gfx::shader::SlangShaderCompiler::create();
        auto graphics_pipeline = GraphicsPipeline::create(device.logical_device, physical_device, swap_chain.surface_format, shader_compiler);
        auto sampler = createTextureSampler(device.logical_device, physical_device);
        auto depth_resource = createDepthResources(allocator.get(), device.logical_device, physical_device, swap_chain.extent);
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
                .sampler = std::move(sampler),
                .depth_resource = std::move(depth_resource)
            }
        };
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
