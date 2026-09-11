//
// Created by Wangsicong Wei on 2026-08-28.
//

#pragma once
#include "RenderPass.h"
#include "VulkanDevice.h"
#include "VulkanInclude.h"
#include "VulkanSwapChain.h"
#include "resource_management/VramVector.h"

namespace vva::gfx::vulkan {
    struct VulkanDevice;

    struct TimeInfo {
        float delta_time;
        float time;
    };


    struct VulkanFrameResource {
        vk::raii::CommandBuffer command_buffer;
        vk::raii::Semaphore image_available_semaphore;
        vk::raii::Fence in_flight_fences;
    };

    struct FrameState {
        uint32_t image_index;
        uint32_t frame_index;
        TimeInfo time_info;
        const VulkanFrameResource& frame_resource;
    };

    struct VulkanFrameController {
        static auto create(const VulkanDevice& device) -> VulkanFrameController;

        auto startFrame(TimeInfo time_info,
                        const VulkanDevice& device,
                        const VulkanSwapChain& swap_chain) -> FrameState;

        auto endFrame(const FrameState& frame_state_store,
                      const VulkanDevice& device,
                      const VulkanSwapChain& swap_chain) -> void;

    private:
        VulkanFrameController(
            vk::raii::CommandPool pool,
            std::vector<VulkanFrameResource> frames)
            : command_pool_(std::move(pool)),
              frame_resources_(std::move(frames)) {
        }

        uint32_t frame_index_{0};
        vk::raii::CommandPool command_pool_;
        std::vector<VulkanFrameResource> frame_resources_;

        [[nodiscard]] auto frame() const -> const VulkanFrameResource&;
        auto advanceFrame() -> void;
        auto getFrameIndex() -> uint32_t { return frame_index_; }
    };
}
