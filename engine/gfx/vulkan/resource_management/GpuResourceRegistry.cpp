//
// Created by Wangsicong Wei on 2026-09-02.
//

#include "GpuResourceRegistry.h"

#include "gfx/vulkan/VulkanAllocator.h"
#include "gfx/vulkan/VulkanDevice.h"

namespace vva::gfx::vulkan {
    auto TextureHandle::getTexture() -> Texture2D& {
        return gpu_resource_registry->textures[id];
    }

    auto GpuResourceRegistry::retrieveMeshHandle(std::string_view id) -> std::expected<MeshHandle, std::string> {
        if (id_to_mesh_handle.contains(id)) {
            return id_to_mesh_handle.at(id);
        } else {
            std::filesystem::path mesh_path {id};
            if (!std::filesystem::exists(mesh_path)) {
                return std::unexpected<std::string>{"path for mesh does not exist"};
            }
            auto mesh = loadModel(id, allocator_.get(), device_, context_);
            return registerMesh(std::move(mesh));
        }
    }

    auto GpuResourceRegistry::retrieveTextureHandle(std::string_view id) -> std::expected<TextureHandle, std::string> {
        if (id_to_texture_handle.contains(id)) {
            return id_to_texture_handle.at(id);
        }
        else {
            std::filesystem::path texture_path{id};
            if (!std::filesystem::exists(texture_path)) {
                return std::unexpected<std::string>{"path for texture does not exist"};
            }
            auto texture = loadTextureFromFile(allocator_.get(), device_, context_, texture_path);
            return registerTexture(std::move(texture));
        }
    }

    auto GpuResourceRegistry::registerMesh(Mesh mesh) -> MeshHandle {
        meshes.push_back(std::move(mesh));
        return {
            meshes.size() - 1
        };
    }

    auto GpuResourceRegistry::registerTexture(Texture2D texture) -> TextureHandle {
        if (textures.size() >= VulkanRenderConfig::MAX_BINDLESS_TEXTURE) {
            throw std::runtime_error("Too much bindless textures. Implement gpu texture eviction.");
        }
        const uint32_t slot = textures.size();

        texture_sampler_descriptor_set_.writeTexture(device_.logical_device, slot, *texture.image_view);

        texture.slot = slot;
        textures.push_back(std::move(texture));
        return TextureHandle {
            this,
            slot,
            true
        }; // this is what goes in Material.albedoTex
    }
}
