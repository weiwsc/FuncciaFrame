//
// Created by Wangsicong Wei on 2026-09-03.
//

#include "FrameSceneDataDescriptorSet.h"

namespace vva::gfx::vulkan {
    auto FrameSceneDataDescriptorSet::create(const vk::raii::Device& device) -> FrameSceneDataDescriptorSet {
        std::array layout_bindings{
            vk::DescriptorSetLayoutBinding{
                .binding = 0,
                .descriptorType = vk::DescriptorType::eUniformBuffer,
                .descriptorCount = 1,
                .stageFlags = vk::ShaderStageFlagBits::eAll
            }
        };
        auto layout_create_info = vk::DescriptorSetLayoutCreateInfo{
            .bindingCount = static_cast<uint32_t>(layout_bindings.size()),
            .pBindings = layout_bindings.data()
        };
        auto layout = vk::raii::DescriptorSetLayout{
            device, layout_create_info
        };

        const std::array pool_sizes{
            vk::DescriptorPoolSize{
                .type = vk::DescriptorType::eUniformBuffer,
                .descriptorCount = VulkanRenderConfig::MAX_FRAME_IN_FLIGHT
            }
        };
        auto pool = vk::raii::DescriptorPool{
            device,
            vk::DescriptorPoolCreateInfo{
                .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
                .maxSets = VulkanRenderConfig::MAX_FRAME_IN_FLIGHT,
                .poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
                .pPoolSizes = pool_sizes.data()
            }
        };
        std::vector<vk::DescriptorSetLayout> layouts(VulkanRenderConfig::MAX_FRAME_IN_FLIGHT, layout);
        std::vector<vk::raii::DescriptorSet> descriptor_sets = device.allocateDescriptorSets(vk::DescriptorSetAllocateInfo{
            .descriptorPool = pool,
            .descriptorSetCount = static_cast<uint32_t>(layouts.size()),
            .pSetLayouts = layouts.data()
        });
        return {
            .frame_uniform_buffer = {},
            .device = &device,
            .layout = std::move(layout),
            .pool = std::move(pool),
            .sets = std::move(descriptor_sets)
        };
    }

    auto FrameSceneDataDescriptorSet::writeFrameData(vk::Buffer buffer, vk::DeviceSize range, uint32_t frame_index) const -> void {
        const vk::DescriptorBufferInfo buffer_info {
            .buffer = buffer,
            .offset = 0,
            .range = sizeof(shader::param::FrameUniformBuffer)
        };
        std::array descriptor_writes{
            vk::WriteDescriptorSet{
                .dstSet = sets[frame_index], .dstBinding = 0, .dstArrayElement = 0, .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eUniformBuffer, .pBufferInfo = &buffer_info
            }
        };
        device->updateDescriptorSets(descriptor_writes, {});
    }
}
