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

namespace vva::gfx::vulkan{
    using SlangShaderCompiler = gfx::shader::SlangShaderCompiler;

    struct GraphicsPipeline {
        vk::raii::PipelineLayout layout;
        vk::raii::Pipeline handle;

        static auto create(const vk::raii::Device& device,
                           const vk::raii::PhysicalDevice& physical_device, const vk::SurfaceFormatKHR& surface_format,
                           SlangShaderCompiler& compiler,
                           const GlobalDescriptors& global_descriptors) -> GraphicsPipeline;
    };

    auto findDepthFormat(const vk::raii::PhysicalDevice& physical_device) -> vk::Format;
    auto createDepthResources(const VmaAllocator& allocator, const vk::raii::Device& device,
                              const vk::raii::PhysicalDevice& physical_device, vk::Extent2D size) -> Texture2D;
}
