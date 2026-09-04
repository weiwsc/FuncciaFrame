//
// Created by Wangsicong Wei on 2026-09-04.
//

#pragma once
import std;
#include "Mesh.h"
#include "Texture.h"
#include "gfx/Transform.h"

namespace vva::gfx::vulkan {
    struct ModelLoadInfo {
        Transform transform{};
        std::string mesh_path;
        std::string texture_path;
    };

    struct ModelHandle {
        uint32_t handle;
    };

    struct Model {
        std::vector<Transform> transforms;
        std::vector<Mesh> meshes;
        std::vector<Texture2D> textures;

        auto createModel(ModelLoadInfo load_info,
                         VmaAllocator allocator,
                         const VulkanDevice& device,
                         VulkanUploadContext& upload) -> std::expected<ModelHandle, std::string> {
            std::filesystem::path mesh_path{load_info.mesh_path};
            std::filesystem::path texture_path{load_info.texture_path};
            if (!std::filesystem::exists(mesh_path) || !std::filesystem::exists(texture_path)) {
                return std::unexpected<std::string>{"path for mesh or texture does not exist"};
            }
            transforms.push_back(load_info.transform);
            meshes.push_back(loadModel(load_info.mesh_path, allocator, device, upload));
            textures.push_back(loadTextureFromFile(allocator, device, upload, texture_path));
            return ModelHandle{
                static_cast<uint32_t>(transforms.size() - 1)
            };
        };
    };
}
