//
// Created by Wangsicong Wei on 2026-08-31.
//

#include "DescriptorAllocator.h"
namespace vva::gfx::vulkan{
    auto createDescriptorPool(const vk::raii::Device& device) -> vk::raii::DescriptorPool {
        std::array pool_size{
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, VulkanRenderConfig::MAX_FRAME_IN_FLIGHT),
            vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, VulkanRenderConfig::MAX_FRAME_IN_FLIGHT)
        };
        vk::DescriptorPoolCreateInfo pool_info{
            .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
            .maxSets = VulkanRenderConfig::MAX_FRAME_IN_FLIGHT,
            .poolSizeCount = static_cast<uint32_t>(pool_size.size()),
            .pPoolSizes = pool_size.data()
        };
        return {device, pool_info};
    }

    auto createDescriptorSets(const vk::raii::DescriptorSetLayout& descriptor_set_layout,
        const vk::raii::DescriptorPool& descriptor_pool, const vk::raii::Device& device,
        const vk::raii::Sampler& sampler, const vk::raii::ImageView& texture_image_view,
        std::span<vk::Buffer> uniform_buffers) -> std::vector<vk::raii::DescriptorSet> {
        std::vector<vk::DescriptorSetLayout> layouts(VulkanRenderConfig::MAX_FRAME_IN_FLIGHT, *descriptor_set_layout);
        vk::DescriptorSetAllocateInfo alloc_info{
            .descriptorPool = descriptor_pool,
            .descriptorSetCount = static_cast<uint32_t>(layouts.size()),
            .pSetLayouts = layouts.data()
        };
        std::vector<vk::raii::DescriptorSet> descriptor_sets;
        descriptor_sets.clear();
        descriptor_sets = device.allocateDescriptorSets(alloc_info);
        for (size_t i = 0; i < VulkanRenderConfig::MAX_FRAME_IN_FLIGHT; i++) {
            vk::DescriptorBufferInfo buffer_info{
                .buffer = uniform_buffers[i], .offset = 0, .range = sizeof(shader::param::FrameUniformBuffer)
            };
            vk::DescriptorImageInfo image_info{
                .sampler = sampler,
                .imageView = texture_image_view,
                .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
            };
            std::array descriptor_writes{
                vk::WriteDescriptorSet{
                    .dstSet = descriptor_sets[i], .dstBinding = 0, .dstArrayElement = 0, .descriptorCount = 1,
                    .descriptorType = vk::DescriptorType::eUniformBuffer, .pBufferInfo = &buffer_info
                },
                vk::WriteDescriptorSet{
                    .dstSet = descriptor_sets[i], .dstBinding = 1, .dstArrayElement = 0, .descriptorCount = 1,
                    .descriptorType = vk::DescriptorType::eCombinedImageSampler, .pImageInfo = &image_info
                }
            };
            device.updateDescriptorSets(descriptor_writes, {});
        }
        return descriptor_sets;
    }
}
