//
// Created by Wangsicong Wei on 2026-09-03.
//

#include "TextureSamplerDescriptorSet.h"

#include "../VulkanConfig.h"

namespace vva::gfx::vulkan {
    auto TextureSamplerDescriptorSet::create(const vk::raii::Device& device) -> TextureSamplerDescriptorSet {
        constexpr auto bindless_flags =
            vk::DescriptorBindingFlagBits::eUpdateAfterBind | // rebind texture to a slot already have a texture
            vk::DescriptorBindingFlagBits::ePartiallyBound | //texture slot can be "null"
            vk::DescriptorBindingFlagBits::eUpdateUnusedWhilePending;
        //"trust me bro just update the slot while rendering is going on
        //I guarantee it's not used in the current frame"

        ///This is like writing:
        ///struct TextureSamplerDescriptorSet {
        ///     SampledImage textures[MAX_TEXTURE];
        ///     Sampler samplers[MAX_SAMPLER]
        ///}
        const std::array bindings{
            vk::DescriptorSetLayoutBinding{
                .binding = TextureSamplerDescriptorSet::TEXTURE_BINDING,
                .descriptorType = vk::DescriptorType::eSampledImage,
                .descriptorCount = VulkanRenderConfig::MAX_BINDLESS_TEXTURE,
                .stageFlags = vk::ShaderStageFlagBits::eAll
            },
            vk::DescriptorSetLayoutBinding{
                .binding = TextureSamplerDescriptorSet::SAMPLER_BINDING,
                .descriptorType = vk::DescriptorType::eSampler,
                .descriptorCount = VulkanRenderConfig::MAX_SAMPLERS,
                .stageFlags = vk::ShaderStageFlagBits::eAll
            }
        };

        const std::array<vk::DescriptorBindingFlags, 2> binding_flag_bits{
            bindless_flags, bindless_flags
        };
        //the structure chain basically attaches the additional binding flag info to the base info
        const vk::StructureChain<vk::DescriptorSetLayoutCreateInfo,
                                 vk::DescriptorSetLayoutBindingFlagsCreateInfo> structure_chain{
            {
                .flags = vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
                .bindingCount = static_cast<uint32_t>(bindings.size()),
                .pBindings = bindings.data()
            },
            {
                .bindingCount = static_cast<uint32_t>(binding_flag_bits.size()),
                .pBindingFlags = binding_flag_bits.data()
            }
        };
        auto layout = vk::raii::DescriptorSetLayout{
            device, structure_chain.get<vk::DescriptorSetLayoutCreateInfo>()
        };
        ///in this case since there's only 1 set, the
        ///amount of textures etc in the whole pool and a set is the same,
        ///otherwise it prob need TEXTURE_PER_SET * TOTAL_SET_COUNT
        const std::array pool_sizes{
            vk::DescriptorPoolSize{
                vk::DescriptorType::eSampledImage,
                VulkanRenderConfig::MAX_BINDLESS_TEXTURE
            },
            vk::DescriptorPoolSize{
                vk::DescriptorType::eSampler,
                VulkanRenderConfig::MAX_SAMPLERS
            }
        };
        auto pool = vk::raii::DescriptorPool{
            device, vk::DescriptorPoolCreateInfo{
                .flags = vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind |
                vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
                .maxSets = 1,
                .poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
                .pPoolSizes = pool_sizes.data()
            }
        };

        auto sets = vk::raii::DescriptorSets{
            device, vk::DescriptorSetAllocateInfo{
                .descriptorPool = pool,
                .descriptorSetCount = 1,
                .pSetLayouts = &*layout
            }
        };
        return {
            .device = &device,
            .layout = std::move(layout),
            .pool = std::move(pool),
            .set = std::move(sets.front())
        };
    }

    auto TextureSamplerDescriptorSet::writeSamplers(std::span<const vk::raii::Sampler> samplers) const -> void {
        std::vector<vk::DescriptorImageInfo> infos;
        for (const auto& s : samplers) infos.push_back({.sampler = *s});
        device->updateDescriptorSets(
            vk::WriteDescriptorSet{
                .dstSet = set, .dstBinding = SAMPLER_BINDING, .dstArrayElement = 0,
                .descriptorCount = static_cast<uint32_t>(infos.size()),
                .descriptorType = vk::DescriptorType::eSampler, .pImageInfo = infos.data()
            }, {});
    }

    auto TextureSamplerDescriptorSet::writeTexture(const uint32_t slot, const vk::ImageView view) const -> void {
        const vk::DescriptorImageInfo image_info{
            .imageView = view,
            .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        };
        device->updateDescriptorSets(
            vk::WriteDescriptorSet{
                .dstSet = set,
                .dstBinding = TEXTURE_BINDING,
                .dstArrayElement = slot,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eSampledImage,
                .pImageInfo = &image_info
            }, {});
    }

    auto TextureSamplerDescriptorSet::writeTexture(uint32_t slot, std::span<const vk::ImageView> views) const -> void {
        std::vector<vk::DescriptorImageInfo> image_infos;
        image_infos.reserve(views.size());
        for (auto& view : views) {
            image_infos.push_back(
                vk::DescriptorImageInfo{
                    .imageView = view,
                    .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
                }
            );
        }
        device->updateDescriptorSets(
            vk::WriteDescriptorSet{
                .dstSet = set,
                .dstBinding = TEXTURE_BINDING,
                .dstArrayElement = slot,
                .descriptorCount = static_cast<uint32_t>(image_infos.size()),
                .descriptorType = vk::DescriptorType::eSampledImage,
                .pImageInfo = image_infos.data()
            }, {});
    }
}
