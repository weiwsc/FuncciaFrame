//
// Created by Wangsicong Wei on 2026-09-03.
//

#pragma once
#include "../VulkanConfig.h"
#include "../VulkanInclude.h"
#include "../types/Param.h"
#include "gfx/vulkan/AllocatedBuffer.h"

namespace vva::gfx::vulkan {
    struct FrameSceneDataDescriptorSet {

        vk::raii::DescriptorSetLayout layout{nullptr};
        vk::raii::DescriptorPool pool{nullptr};
        std::vector<vk::raii::DescriptorSet> sets;
        std::vector<AllocatedBuffer> frame_uniform_buffers_allocation;

        static auto create(const vk::raii::Device& device, VmaAllocator allocator) -> FrameSceneDataDescriptorSet;

        auto writeFrameUniform(const shader::param::FrameUniformBuffer& data, uint32_t frame_index) const -> void;
    };
}
