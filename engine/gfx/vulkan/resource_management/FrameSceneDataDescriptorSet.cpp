//
// Created by Wangsicong Wei on 2026-09-03.
//

#include "FrameSceneDataDescriptorSet.h"

#include "gfx/vulkan/VulkanAllocator.h"

namespace vva::gfx::vulkan {


    auto FrameSceneDataDescriptorSet::create(const vk::raii::Device& device,
                                             VmaAllocator allocator) -> FrameSceneDataDescriptorSet {
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
        std::vector<vk::raii::DescriptorSet> descriptor_sets = device.allocateDescriptorSets(
            vk::DescriptorSetAllocateInfo{
                .descriptorPool = pool,
                .descriptorSetCount = static_cast<uint32_t>(layouts.size()),
                .pSetLayouts = layouts.data()
            });


        std::vector<AllocatedBuffer> buffers;
        buffers.reserve(VulkanRenderConfig::MAX_FRAME_IN_FLIGHT);
        for (uint32_t i = 0; i < VulkanRenderConfig::MAX_FRAME_IN_FLIGHT; ++i) {
            buffers.push_back(AllocatedBuffer::createBuffer(
                allocator,
                sizeof(shader::param::FrameUniformBuffer),
                vk::BufferUsageFlagBits::eUniformBuffer,
                VMA_MEMORY_USAGE_AUTO,
                VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT));

            const vk::DescriptorBufferInfo info{
                .buffer = buffers[i].handle(), .offset = 0, .range = sizeof(shader::param::FrameUniformBuffer)
            };
            device.updateDescriptorSets(
                vk::WriteDescriptorSet{
                    .dstSet = descriptor_sets[i], .dstBinding = 0, .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = vk::DescriptorType::eUniformBuffer, .pBufferInfo = &info
                }, {});
        }


        return {
            .layout = std::move(layout),
            .pool = std::move(pool),
            .sets = std::move(descriptor_sets),
            .frame_uniform_buffers_allocation = std::move(buffers)
        };
    }

    auto FrameSceneDataDescriptorSet::writeFrameUniform(const shader::param::FrameUniformBuffer& data,
                                                        uint32_t frame_index) const -> void {
        const auto& buffer = frame_uniform_buffers_allocation[frame_index];
        std::memcpy(buffer.mappedData(), &data, sizeof(data));
        buffer.flush();
    }
}
