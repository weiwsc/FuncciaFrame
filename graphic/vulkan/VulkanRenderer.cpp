//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanRenderer.h"

#include <SDL3/SDL_vulkan.h>

#include "../../util/Log.h"
#include "util/VulkanLogicalDeviceUtil.h"
#include "util/VulkanPhysicalDeviceUtil.h"

namespace Funccia::Graphic::Vulkan {

VulkanRenderer::VulkanRenderer(VulkanInstance instance, VulkanDevice device, vk::raii::SurfaceKHR surface, VulkanSwapChain swap_chain)
    : instance(std::move(instance)),
        device(std::move(device)),
        surface((std::move(surface))),
        swap_chain(std::move(swap_chain)){

}

void VulkanRenderer::Init(WindowInterface& window) {
    // TODO: deprecate init() style intialization in the interface
}

auto VulkanRenderer::CreateVulkanRenderer(WindowInterface& window_interface) -> VulkanRenderer {
        auto instance = VulkanInstance::createVulkanInstance(true);
        auto surface = CreateSurface(window_interface, instance.instance);
        auto physical_device_selection_result = Util::pickPhysicalDevice(instance.instance, surface);
        auto physical_device = physical_device_selection_result.physical_device;
        auto device = Util::CreateVulkanDevice(physical_device, physical_device_selection_result.queue_coordinates);
        ivec2 buffer_size {};
        window_interface.GetFramebufferSize(buffer_size.x, buffer_size.y);
        auto swap_chain = VulkanSwapChain::CreateSwapChian(surface, physical_device, device.logical_device, buffer_size);
        vva_log_info("vulkan renderer created");
        return VulkanRenderer {
            (std::move(instance)),
            (std::move(device)),
            (std::move(surface)),
            std::move(swap_chain)
        };
    }

    auto VulkanRenderer::CreateSurface(WindowInterface& window,
                                       const vk::raii::Instance& instance)->vk::raii::SurfaceKHR {
        VkSurfaceKHR _surface;
        if (!SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(window.GetNativeWindow()), *instance, nullptr,
                                      &_surface)) {
            throw std::runtime_error("failed to create window surface!");
                                      }
        return vk::raii::SurfaceKHR(instance, _surface);
    }
}
