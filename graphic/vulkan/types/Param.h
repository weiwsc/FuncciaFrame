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
namespace Funccia::Graphic::Shader::Param {
    struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
    alignas(16) float time;
};
struct UniformBufferObject2 {
    alignas(8) glm::vec2 resolution;
    alignas(8) glm::vec2 mouse;
    alignas(4) float time;
    float  exposure;     // ~1.0
    float  density;      // ~1.0
    float  speed;        // ~1.0
    float  starBoost;    // ~1.0
};
    struct PosNormalUV {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texCoord;

        bool operator==(const PosNormalUV& other) const {
            return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
        }
        static vk::VertexInputBindingDescription getBindingDescription() {
            return { 0, sizeof(PosNormalUV), vk::VertexInputRate::eVertex };
        }

        static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions() {
            return {
                vk::VertexInputAttributeDescription( 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(PosNormalUV, pos) ),
                vk::VertexInputAttributeDescription( 1, 0, vk::Format::eR32G32B32Sfloat, offsetof(PosNormalUV, normal) ),
                vk::VertexInputAttributeDescription( 2, 0, vk::Format::eR32G32Sfloat, offsetof(PosNormalUV, texCoord) )
            };
        }
    };

}
template<> struct std::hash<Funccia::Graphic::Shader::Param::PosNormalUV> {
    size_t operator()(const Funccia::Graphic::Shader::Param::PosNormalUV& vertex) const { /* same body */ }
};