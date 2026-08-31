//
// Created by Wangsicong Wei on 2026-08-31.
//

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>
#include "Model.h"

#include "Param.h"
#include "../VulkanDevice.h"
#include "../VulkanUploadContext.h"

namespace Funccia::Graphic::Vulkan {
    namespace {
        using Vertex = Shader::Param::PosNormalUV;

        struct ModelLoadResult {
            std::vector<Vertex> vertex_buffer;
            std::vector<uint32_t> index_buffer;
        };

        auto loadModel(std::string model_path) -> ModelLoadResult {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string warn, err;

            if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.c_str())) {
                throw std::runtime_error(warn + err);
            }
            std::unordered_map<Vertex, uint32_t> uniqueVertices{};
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            for (const auto& shape : shapes) {
                for (const auto& index : shape.mesh.indices) {
                    Vertex vertex{};
                    vertex.pos = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    };
                    vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                    vertex.texCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                    if (!uniqueVertices.contains(vertex)) {
                        uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex);
                    }
                    // vertices.push_back(vertex);
                    indices.push_back(uniqueVertices[vertex]);
                }
            }
            return {
                std::move(vertices),
                std::move(indices)
            };
        }

        auto CreateModel(
            VmaAllocator allocator,
            const VulkanDevice& device,
            VulkanUploadContext& upload,
            const ModelLoadResult& data) -> Model {
            const vk::DeviceSize vertexBytes =
                data.vertex_buffer.size() * sizeof(Vertex);
            const vk::DeviceSize indexBytes =
                data.index_buffer.size() * sizeof(uint32_t);

            auto makeStaging = [&](const void* src, vk::DeviceSize size) {
                auto buffer = AllocatedBuffer::CreateBuffer(
                    allocator, size,
                    vk::BufferUsageFlagBits::eTransferSrc,
                    VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
                    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                    VMA_ALLOCATION_CREATE_MAPPED_BIT);
                std::memcpy(buffer.mappedData(), src, size);
                vmaFlushAllocation(allocator, buffer.allocation(), 0, VK_WHOLE_SIZE);
                return buffer;
            };

            auto vertexStaging = makeStaging(data.vertex_buffer.data(), vertexBytes);
            auto indexStaging = makeStaging(data.index_buffer.data(), indexBytes);

            auto vertexBuffer = AllocatedBuffer::CreateBuffer(
                allocator, vertexBytes,
                vk::BufferUsageFlagBits::eTransferDst |
                vk::BufferUsageFlagBits::eVertexBuffer,
                VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE);

            auto indexBuffer = AllocatedBuffer::CreateBuffer(
                allocator, indexBytes,
                vk::BufferUsageFlagBits::eTransferDst |
                vk::BufferUsageFlagBits::eIndexBuffer,
                VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE);

            const auto& cmd = upload.command_buffer;
            device.logical_device.resetFences(*upload.fence);
            cmd.reset();
            cmd.begin({.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit});

            cmd.copyBuffer(vertexStaging.handle(), vertexBuffer.handle(),
                           vk::BufferCopy{0, 0, vertexBytes});
            cmd.copyBuffer(indexStaging.handle(), indexBuffer.handle(),
                           vk::BufferCopy{0, 0, indexBytes});

            // Make the transfer writes visible to vertex/index fetch in later submits.
            vk::MemoryBarrier2 copyToRead{
                .srcStageMask = vk::PipelineStageFlagBits2::eCopy,
                .srcAccessMask = vk::AccessFlagBits2::eTransferWrite,
                .dstStageMask = vk::PipelineStageFlagBits2::eVertexAttributeInput |
                vk::PipelineStageFlagBits2::eIndexInput,
                .dstAccessMask = vk::AccessFlagBits2::eVertexAttributeRead |
                vk::AccessFlagBits2::eIndexRead
            };
            cmd.pipelineBarrier2({
                .memoryBarrierCount = 1,
                .pMemoryBarriers = &copyToRead
            });

            cmd.end();

            const vk::SubmitInfo submit{
                .commandBufferCount = 1,
                .pCommandBuffers = &*cmd
            };
            device.queues.graphics_queue_.submit(submit, *upload.fence);
            (void)device.logical_device.waitForFences(*upload.fence, vk::True, UINT64_MAX);

            // Staging buffers destroyed here by RAII — safe, the fence already waited.
            return Model{
                .index_count = static_cast<uint32_t>(data.index_buffer.size()),
                .vertex_buffer = std::move(vertexBuffer),
                .index_buffer = std::move(indexBuffer)
            };
        }
    }

    auto loadModel(std::string_view model_name,
                   VmaAllocator allocator,
                   const VulkanDevice& device,
                   VulkanUploadContext& upload) -> Model
    {
        const ModelLoadResult data = loadModel(std::string{model_name});
        return CreateModel(allocator, device, upload, data);
    }
}
