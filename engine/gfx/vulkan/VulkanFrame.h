//
// Created by Wangsicong Wei on 2026-08-28.
//

#pragma once
#include "VulkanInclude.h"

namespace vva::gfx::vulkan{
    struct VulkanDevice;

    struct VulkanFrameResource {
        vk::raii::CommandBuffer command_buffer;
        vk::raii::Semaphore image_available_semaphore;
        vk::raii::Fence in_flight_fences;
    };

    struct VulkanFrameController {
        static auto create(const VulkanDevice& device) -> VulkanFrameController;
        [[nodiscard]] auto frame() const -> const VulkanFrameResource&;
        auto advanceFrame() -> void;
        auto getFrameIndex() -> uint32_t {return frame_index_;};
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
    };
}
