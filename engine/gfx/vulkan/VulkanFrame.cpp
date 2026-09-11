//
// Created by Wangsicong Wei on 2026-08-28.
//

#include "VulkanFrame.h"

#include "VulkanConfig.h"
#include "VulkanDevice.h"
#include "core/Log.h"

namespace vva::gfx::vulkan {
    auto VulkanFrameController::create(const VulkanDevice& device) -> VulkanFrameController {
        const vk::CommandPoolCreateInfo pool_info{
            .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            .queueFamilyIndex = device.queue_coordinates.graphics.queue_family_index
        };
        auto command_pool = vk::raii::CommandPool(device.logical_device, pool_info);


        vk::CommandBufferAllocateInfo alloc_info{
            .commandPool = command_pool,
            .level = vk::CommandBufferLevel::ePrimary,
            .commandBufferCount = VulkanRenderConfig::MAX_FRAME_IN_FLIGHT
        };
        auto command_buffers = vk::raii::CommandBuffers(device.logical_device, alloc_info);


        std::vector<VulkanFrameResource> frame_resources;
        frame_resources.reserve(VulkanRenderConfig::MAX_FRAME_IN_FLIGHT);

        for (int i = 0; i < VulkanRenderConfig::MAX_FRAME_IN_FLIGHT; ++i) {
            frame_resources.emplace_back(
                VulkanFrameResource{
                    .command_buffer = std::move(command_buffers[i]),
                    .image_available_semaphore = {device.logical_device, vk::SemaphoreCreateInfo()},
                    .in_flight_fences = {
                        device.logical_device, vk::FenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled}
                    }
                });
        }
        vva_log_info("frame controller created, total frame resource: {}", VulkanRenderConfig::MAX_FRAME_IN_FLIGHT);
        return {
            std::move(command_pool),
            std::move(frame_resources)
        };
    }

    auto VulkanFrameController::frame() const -> const VulkanFrameResource& {
        return frame_resources_[frame_index_];
    }

    auto VulkanFrameController::advanceFrame() -> void {
        frame_index_ = (frame_index_ + 1) % VulkanRenderConfig::MAX_FRAME_IN_FLIGHT;
    }

    auto VulkanFrameController::startFrame(TimeInfo time_info,
                                           const VulkanDevice& device,
                                           const VulkanSwapChain& swap_chain) -> FrameState {

        //graphicsQueue.waitIdle();
        auto frame_index = getFrameIndex();
        auto& frame_resource = frame();
        auto fence_result = device.logical_device.waitForFences(*frame_resource.in_flight_fences, vk::True,
                                                                UINT64_MAX);
        if (fence_result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to wait for fence!");
        }

        auto [result, imageIndex] = swap_chain.handle.acquireNextImage(
            UINT64_MAX, *frame_resource.image_available_semaphore, nullptr);
        //frame_state_store.image_index = imageIndex;
        // Due to VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS being defined, eErrorOutOfDateKHR can be checked as a result
        // here and does not need to be caught by an exception.
        // if (result == vk::Result::eErrorOutOfDateKHR) {
        //     recreateSwapChain();
        //     return;
        // }
        // On other success codes than eSuccess and eSuboptimalKHR we just throw an exception.
        // On any error code, aquireNextImage already threw an exception.
        if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
            assert(result == vk::Result::eTimeout || result == vk::Result::eNotReady);
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        // Only reset the fence if we are submitting work
        device.logical_device.resetFences(*frame_resource.in_flight_fences);
        frame_resource.command_buffer.reset();

        auto& command_buffer = frame_resource.command_buffer;
        // Before starting rendering, transition the swapchain image to COLOR_ATTACHMENT_OPTIMAL
        command_buffer.begin({});


        return {
            .image_index = imageIndex,
            .frame_index = frame_index,
            .time_info = time_info,
            .frame_resource = frame_resource
        };
    }

    auto VulkanFrameController::endFrame(const FrameState& frame_state_store,
                                         const VulkanDevice& device,
                                         const VulkanSwapChain& swap_chain) -> void {
        auto frame_index = getFrameIndex();
        auto& frame_resource = frame_state_store.frame_resource;
        auto& command_buffer = frame_state_store.frame_resource.command_buffer;

        const VulkanSwapChainImage& image = swap_chain.images[frame_state_store.image_index];

        command_buffer.end();

        const auto& render_finished_semaphore = swap_chain.images[frame_state_store.image_index].
            render_finished_semaphore;

        vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
        const vk::SubmitInfo submitInfo{
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &*frame_resource.image_available_semaphore,
            .pWaitDstStageMask = &waitDestinationStageMask,
            .commandBufferCount = 1,
            .pCommandBuffers = &*command_buffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &*render_finished_semaphore
        };
        device.queues.graphics_queue.submit(submitInfo, *frame_resource.in_flight_fences);
        // auto result = context_.device.logical_device.waitForFences(*frame_resource.in_flight_fences, vk::True, UINT64_MAX);
        //
        // if (result != vk::Result::eSuccess) {
        //     throw std::runtime_error("failed to wait for fence!");
        // }

        const vk::PresentInfoKHR presentInfoKHR{
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &*render_finished_semaphore,
            .swapchainCount = 1,
            .pSwapchains = &*swap_chain.handle,
            .pImageIndices = &frame_state_store.image_index
        };
        auto result = device.queues.present_queue.presentKHR(presentInfoKHR);

        // Due to VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS being defined, eErrorOutOfDateKHR can be checked as a result
        // here and does not need to be caught by an exception.
        // if ((result == vk::Result::eSuboptimalKHR) || (result == vk::Result::eErrorOutOfDateKHR) ||
        //     framebufferResized) {
        //     framebufferResized = false;
        //     recreateSwapChain();
        //     } else {
        //         // There are no other success codes than eSuccess; on any error code, presentKHR already threw an exception.
        //         assert(result == vk::Result::eSuccess);
        //     }
        advanceFrame();
    }
}
