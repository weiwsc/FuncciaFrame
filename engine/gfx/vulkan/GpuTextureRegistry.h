//
// Created by Wangsicong Wei on 2026-09-02.
//

#pragma once
#include "VulkanConfig.h"
#include "VulkanInclude.h"
#include "types/Texture.h"

namespace vva::gfx::vulkan {
    struct GpuTextureRegistry {
        std::vector<Texture2D> textures;

        uint32_t registerTexture(Texture2D texture, const vk::raii::Device& device, const vk::raii::DescriptorSet& bindless_descriptor_set) {
            if (textures.size() >= VulkanRenderConfig::MAX_BINDLESS_TEXTURE) {
                throw std::runtime_error("Too much bindless textures. Implement gpu texture eviction.");
            }
            const uint32_t slot = textures.size();
            vk::DescriptorImageInfo img{
                .imageView = texture.image_view,
                .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
            };
            vk::WriteDescriptorSet w{
                .dstSet = bindless_descriptor_set,
                .dstBinding = 1, .dstArrayElement = slot, .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eSampledImage, .pImageInfo = &img
            };
            vkUpdateDescriptorSets(*device, 1, &*w, 0, nullptr);
            texture.slot = slot;
            textures.push_back(std::move(texture));
            return slot; // this is what goes in Material.albedoTex
        }
    };
}
