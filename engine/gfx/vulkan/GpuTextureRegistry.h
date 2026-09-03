//
// Created by Wangsicong Wei on 2026-09-02.
//

#pragma once
#include "VulkanConfig.h"
#include "VulkanInclude.h"
#include "resource_management/TextureSamplerDescriptorSet.h"
#include "types/Texture.h"

namespace vva::gfx::vulkan {
    struct GpuTextureRegistry {
        std::vector<Texture2D> textures;

        uint32_t registerTexture(Texture2D texture, const vk::raii::Device& device, const TextureSamplerDescriptorSet& texture_sampler_descriptor_set) {
            if (textures.size() >= VulkanRenderConfig::MAX_BINDLESS_TEXTURE) {
                throw std::runtime_error("Too much bindless textures. Implement gpu texture eviction.");
            }
            const uint32_t slot = textures.size();

            texture_sampler_descriptor_set.writeTexture(slot, *texture.image_view);

            texture.slot = slot;
            textures.push_back(std::move(texture));
            return slot; // this is what goes in Material.albedoTex
        }
    };
}
