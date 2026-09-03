//
// Created by Wangsicong Wei on 2026-09-02.
//

#pragma once
#include "../VulkanInclude.h"

namespace vva::gfx::vulkan {
    enum SamplerId : uint32_t {
    SAMPLER_LINEAR_REPEAT = 0,
    SAMPLER_LINEAR_CLAMP,
    SAMPLER_NEAREST_REPEAT,
    SAMPLER_NEAREST_CLAMP,
    SAMPLER_ANISO_REPEAT,
    SAMPLER_SHADOW,
    SAMPLER_COUNT
};

auto createSampler(const vk::raii::Device& device, vk::Filter filter, vk::SamplerAddressMode addr,
                   float aniso = 0.f, bool compare = false) -> vk::raii::Sampler ;

auto createSamplers(const vk::raii::Device& device) -> std::vector<vk::raii::Sampler> ;

void registerSamplers(const vk::raii::Device& device, const vk::raii::DescriptorSet& bindlessSet,
                      std::span<const vk::raii::Sampler> samplers);
}
