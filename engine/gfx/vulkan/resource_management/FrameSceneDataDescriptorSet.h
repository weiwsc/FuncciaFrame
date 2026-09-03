//
// Created by Wangsicong Wei on 2026-09-03.
//

#pragma once
#include "../VulkanConfig.h"
#include "../VulkanInclude.h"
#include "../types/Param.h"

namespace vva::gfx::vulkan {
    struct FrameSceneDataDescriptorSet {
        shader::param::FrameUniformBuffer frame_uniform_buffer;

        const vk::raii::Device* device {nullptr};
        vk::raii::DescriptorSetLayout layout{nullptr};
        vk::raii::DescriptorPool pool{nullptr};
        std::vector<vk::raii::DescriptorSet> sets;

        static auto create(const vk::raii::Device& device) -> FrameSceneDataDescriptorSet;

        auto writeFrameData(vk::Buffer buffer, vk::DeviceSize range, uint32_t frame_index) const -> void;
    };
}
