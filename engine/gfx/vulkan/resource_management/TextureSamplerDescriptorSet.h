//
// Created by Wangsicong Wei on 2026-09-03.
//

#pragma once
#include "../VulkanInclude.h"

namespace vva::gfx::vulkan {
    struct TextureSamplerDescriptorSet {
        static constexpr uint32_t TEXTURE_BINDING = 0;
        static constexpr uint32_t SAMPLER_BINDING = 1;

        vk::raii::DescriptorSetLayout layout{nullptr};
        vk::raii::DescriptorPool pool{nullptr};
        vk::raii::DescriptorSet set{nullptr};

        static auto create(const vk::raii::Device& device) -> TextureSamplerDescriptorSet;

        auto writeSamplers(const vk::raii::Device& device, std::span<const vk::raii::Sampler> samplers) const -> void;
        auto writeTexture(const vk::raii::Device& device, uint32_t slot, vk::ImageView view) const -> void;
        auto writeTexture(const vk::raii::Device& device, uint32_t slot, std::span<const vk::ImageView> views) const -> void;
    };
}
