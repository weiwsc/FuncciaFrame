//
// Created by Wangsicong Wei on 2026-09-02.
//

#include "Sampler.h"
namespace vva::gfx::vulkan {
    auto createSampler(const vk::raii::Device& device, vk::Filter filter, vk::SamplerAddressMode addr, float aniso,
        bool compare) -> vk::raii::Sampler {
        vk::SamplerCreateInfo info{
            .magFilter        = filter,
            .minFilter        = filter,
            .mipmapMode       = filter == vk::Filter::eLinear ? vk::SamplerMipmapMode::eLinear
                                    : vk::SamplerMipmapMode::eNearest,
            .addressModeU     = addr,
            .addressModeV     = addr,
            .addressModeW     = addr,
            .anisotropyEnable = aniso > 0.f,
            .maxAnisotropy    = aniso,
            .compareEnable    = compare,
            .compareOp        = vk::CompareOp::eLessOrEqual,
            .minLod           = 0.f,
            .maxLod           = vk::LodClampNone,
            .borderColor      = vk::BorderColor::eFloatOpaqueWhite,  // shadow sampler: outside = lit
        };
        return vk::raii::Sampler{device, info};
    }

    auto createSamplers(const vk::raii::Device& device) -> std::vector<vk::raii::Sampler> {
        using enum vk::Filter;
        using enum vk::SamplerAddressMode;

        std::vector<vk::raii::Sampler> samplers;
        samplers.reserve(SAMPLER_COUNT);
        samplers.emplace_back(createSampler(device, eLinear,  eRepeat));                   // LINEAR_REPEAT
        samplers.emplace_back(createSampler(device, eLinear,  eClampToEdge));              // LINEAR_CLAMP
        samplers.emplace_back(createSampler(device, eNearest, eRepeat));                   // NEAREST_REPEAT
        samplers.emplace_back(createSampler(device, eNearest, eClampToEdge));              // NEAREST_CLAMP
        samplers.emplace_back(createSampler(device, eLinear,  eRepeat, 16.f));             // ANISO_REPEAT
        samplers.emplace_back(createSampler(device, eLinear,  eClampToBorder, 0.f, true)); // SHADOW
        return samplers;
    }

    void registerSamplers(const vk::raii::Device& device, const vk::raii::DescriptorSet& bindlessSet,
        std::span<const vk::raii::Sampler> samplers) {
        std::vector<vk::DescriptorImageInfo> infos;
        infos.reserve(samplers.size());
        for (const auto& s : samplers)
            infos.push_back({ .sampler = *s });   // deref RAII handle → vk::Sampler

        vk::WriteDescriptorSet write{
            .dstSet          = *bindlessSet,
            .dstBinding      = 2,
            .dstArrayElement = 0,
            .descriptorCount = static_cast<uint32_t>(infos.size()),
            .descriptorType  = vk::DescriptorType::eSampler,
            .pImageInfo      = infos.data(),
        };
        device.updateDescriptorSets(write, {});
    }
}
