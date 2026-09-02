//
// Created by Wangsicong Wei on 2026-08-31.
//

#include "DescriptorAllocator.h"
namespace vva::gfx::vulkan{
    auto createDescriptorPool(const vk::raii::Device& device) -> void {
        std::array pool_size{
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, VulkanRenderConfig::max_frame_in_flight),
            vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, VulkanRenderConfig::max_frame_in_flight)
        };
        vk::DescriptorPoolCreateInfo pool_info{
            .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
            .maxSets = VulkanRenderConfig::max_frame_in_flight,
            .poolSizeCount = static_cast<uint32_t>(pool_size.size()),
            .pPoolSizes = pool_size.data()
        };
        auto descriptor_pool = vk::raii::DescriptorPool(device, pool_info);
    }

    auto createDescriptorSets(const vk::raii::DescriptorSetLayout descriptor_set_layout,
        const vk::raii::DescriptorPool& descriptor_pool, const vk::raii::Device& device,
        const vk::raii::Sampler& sampler, const vk::raii::ImageView& image_view,
        std::span<vk::Buffer> uniform_buffers) -> std::vector<vk::raii::DescriptorSet> {
        std::vector<vk::DescriptorSetLayout> layouts(VulkanRenderConfig::max_frame_in_flight, *descriptor_set_layout);
        vk::DescriptorSetAllocateInfo alloc_info{
            .descriptorPool = descriptor_pool,
            .descriptorSetCount = static_cast<uint32_t>(layouts.size()),
            .pSetLayouts = layouts.data()
        };
        std::vector<vk::raii::DescriptorSet> descriptor_sets;
        descriptor_sets.clear();
        descriptor_sets = device.allocateDescriptorSets(alloc_info);
        for (size_t i = 0; i < VulkanRenderConfig::max_frame_in_flight; i++) {
            vk::DescriptorBufferInfo buffer_info{
                .buffer = uniform_buffers[i], .offset = 0, .range = sizeof(shader::param::UniformBufferObject)
            };
            vk::DescriptorImageInfo image_info{
                .sampler = sampler,
                .imageView = image_view,
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
