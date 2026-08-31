//
// Created by Wangsicong Wei on 2026-08-28.
//

#pragma once
#include "VulkanInclude.h"

namespace Funccia::Graphic::Vulkan {
    struct VulkanDevice;

    struct VulkanFrameResource {
        vk::raii::CommandBuffer command_buffer;
        vk::raii::Semaphore present_complete_semaphore;
        vk::raii::Semaphore image_available_semaphore;
        vk::raii::Fence in_flight_fences;
    };

    struct VulkanFrameController {
        static auto Create(const VulkanDevice& device) -> VulkanFrameController;
        [[nodiscard]] auto frame() const -> const VulkanFrameResource&;
        auto advanceFrame() -> void;

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
