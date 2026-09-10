//
// Created by Wangsicong Wei on 2026-08-31.
//

#pragma once
#include "../VulkanInclude.h"
#include "../resource_management/AllocatedBuffer.h"
#include "../resource_management/AllocatedImage.h"

namespace vva::gfx::vulkan{
    struct VulkanUploadContext;
    struct VulkanDevice;
    using Buffer = vva::gfx::vulkan::AllocatedBuffer;

    struct Mesh {
        uint32_t index_count;
        Buffer vertex_buffer;
        Buffer index_buffer;
    };

    auto loadModel(std::string_view model_name,
                   VmaAllocator allocator,
                   const VulkanDevice& device,
                   const VulkanUploadContext& upload) -> Mesh;
}
