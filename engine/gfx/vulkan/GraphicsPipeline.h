//
// Created by Wangsicong Wei on 2026-08-30.
//

#pragma once
#include <vector>

#include "VulkanAllocator.h"
#include "VulkanInclude.h"
#include "SlangShaderCompiler.h"
#include "resource_management/DescriptorTypeDef.h"
#include "types/Texture.h"

namespace vva::gfx::vulkan {
    using SlangShaderCompiler = gfx::shader::SlangShaderCompiler;

    struct GraphicsPipeline {
        vk::raii::PipelineLayout layout;
        vk::raii::Pipeline handle;

        static auto create(
            std::string shader_name, const vk::raii::Device& device,
            const vk::raii::PhysicalDevice& physical_device, const vk::SurfaceFormatKHR& surface_format,
            SlangShaderCompiler& compiler,
            const GlobalDescriptors& global_descriptor,
            bool has_depth_attachments,
            vk::CullModeFlagBits cull_mode = vk::CullModeFlagBits::eBack) -> GraphicsPipeline;
    };

    auto findDepthFormat(const vk::raii::PhysicalDevice& physical_device) -> vk::Format;
    auto createDepthResources(const VmaAllocator& allocator, const vk::raii::Device& device,
                              const vk::raii::PhysicalDevice& physical_device, vk::Extent2D size) -> Texture2D;
    auto createIntermediateResource(const VmaAllocator& allocator,
                                    const vk::raii::Device& device,
                                    const vk::raii::PhysicalDevice& physical_device, vk::Extent2D size) -> Texture2D;
}
