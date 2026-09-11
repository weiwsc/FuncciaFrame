//
// Created by Wangsicong Wei on 2026-08-30.
//

#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../VulkanInclude.h"

namespace vva::gfx::shader::param {
    // struct PushConstants {
    //     alignas(16) glm::mat4 model_matrix;
    //     alignas(8) vk::DeviceAddress material_address;
    //     alignas(8) vk::DeviceAddress object_address;
    // };
    struct BasicDrawData {
        alignas(16) glm::mat4 model_matrix;
        uint64_t texture_index;
        uint64_t sampler_index;
    };
    struct TextureDrawData {
        uint64_t texture_index;
        uint64_t sampler_index;
        glm::vec2 size;
    };
    struct PushConstants {
        vk::DeviceAddress instances; // arena.address() + offset of this frame's array
    };


    struct FrameUniformBuffer {
        alignas(16) glm::mat4 view, projection, view_projection;
        alignas(16) glm::mat4 inverse_view, inverse_projection;
        alignas(16) glm::vec3 camera_position;
        alignas(8) glm::vec2 resolution;
        alignas(8) glm::vec2 mouse;
        alignas(4) float time;
        alignas(4) float delta_time;
        vk::DeviceAddress lights;
        //VkDeviceAddress material; //there might be different materials
    };

    struct CameraBuffer {
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    struct UniformBufferObject2 {
        alignas(8) glm::vec2 resolution;
        alignas(8) glm::vec2 mouse;
        alignas(4) float time;
        float exposure; // ~1.0
        float density; // ~1.0
        float speed; // ~1.0
        float star_boost; // ~1.0
    };

    struct PosNormalUV {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 tex_coord;

        auto operator==(const PosNormalUV& other) const -> bool {
            return pos == other.pos && normal == other.normal && tex_coord == other.tex_coord;
        }

        static auto getBindingDescription() -> vk::VertexInputBindingDescription {
            return {0, sizeof(PosNormalUV), vk::VertexInputRate::eVertex};
        }

        static auto getAttributeDescriptions() -> std::array<vk::VertexInputAttributeDescription, 3> {
            return {
                vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(PosNormalUV, pos)),
                vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(PosNormalUV, normal)),
                vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32Sfloat, offsetof(PosNormalUV, tex_coord))
            };
        }
    };
}

template <>
struct std::hash<vva::gfx::shader::param::PosNormalUV> {
    auto operator()(const vva::gfx::shader::param::PosNormalUV& vertex) const noexcept -> size_t {
        return ((hash<glm::vec3>()(vertex.pos)
                ^ (hash<glm::vec3>()(vertex.normal) << 1)) >> 1)
            ^ (hash<glm::vec2>()(vertex.tex_coord) << 1);
    }
};
