//
// Created by Wangsicong Wei on 2026-08-30.
//

#pragma once
#include <vector>
#include "VulkanInclude.h"
#include "../shaders/SlangShaderCompiler.h"

namespace Funccia::Graphic::Vulkan {
    using SlangShaderCompiler = Graphics::Shader::SlangShaderCompiler;

    struct GraphicsPipeline {
        std::vector<vk::raii::DescriptorSetLayout> descriptor_set_layouts;
        vk::raii::PipelineLayout layout;
        vk::raii::Pipeline handle;

        static auto Create(const vk::raii::Device& device,
                           const vk::raii::PhysicalDevice& physical_device, const vk::SurfaceFormatKHR& surface_format,
                           SlangShaderCompiler& compiler) -> GraphicsPipeline;
    };
}
