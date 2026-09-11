//
// Created by Wangsicong Wei on 2026-09-02.
//

#pragma once
#include <expected>
#include <unordered_map>

#include "../VulkanConfig.h"
#include "../VulkanInclude.h"
#include "TextureSamplerDescriptorSet.h"
#include "../types/Texture.h"
#include "gfx/vulkan/types/Mesh.h"

namespace vva::gfx::vulkan {
    struct GpuResourceRegistry;


    struct MeshHandle {
        uint64_t id;
    };

    struct TextureHandle {
        GpuResourceRegistry* gpu_resource_registry;
        uint64_t id;
        bool exists {false};
        auto getTexture() -> Texture2D&;
    };

    struct GpuResourceRegistry {
        GpuResourceRegistry(const VulkanDevice& device,
                            const TextureSamplerDescriptorSet& texture_sampler_descriptor_set,
                            const VulkanAllocator& allocator,
                            const VulkanUploadContext& upload_context) :
            device_(device),
            texture_sampler_descriptor_set_(texture_sampler_descriptor_set),
            allocator_(allocator),
            context_(upload_context) {
        }

        auto retrieveMeshHandle(std::string_view id) -> std::expected<MeshHandle, std::string>;
        auto retrieveTextureHandle(std::string_view id) -> std::expected<TextureHandle, std::string>;

        auto registerMesh(Mesh mesh) -> MeshHandle;

        auto registerTexture(Texture2D texture) -> TextureHandle;
        std::unordered_map<std::string_view, MeshHandle> id_to_mesh_handle;
        std::unordered_map<std::string_view, TextureHandle> id_to_texture_handle;
        std::vector<Mesh> meshes;
        std::vector<Texture2D> textures;
    private:
        const VulkanDevice& device_;
        const TextureSamplerDescriptorSet& texture_sampler_descriptor_set_;
        const VulkanAllocator& allocator_;
        const VulkanUploadContext& context_;


    };
}
