//
// Created by Wangsicong Wei on 2026-08-31.
//

#pragma once
#include "VulkanConfig.h"
#include "VulkanInclude.h"
#include "types/Param.h"

namespace Funccia::Graphic::Vulkan {
    struct DescriptorAllocator {
        vk::raii::DescriptorPool descriptor_pool;
        std::vector<vk::raii::DescriptorSet> descriptor_sets;
        auto Create();
    };
    auto createDescriptorPool(const vk::raii::Device& device) -> void ;

    auto createDescriptorSets(
    const vk::raii::DescriptorSetLayout descriptor_set_layout,
    const vk::raii::DescriptorPool& descriptor_pool,
        const vk::raii::Device& device,
        const vk::raii::Sampler& sampler,
        const vk::raii::ImageView& image_view,
        std::span<vk::Buffer> uniform_buffers) -> std::vector<vk::raii::DescriptorSet> ;

}
