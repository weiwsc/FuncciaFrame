//
// Created by Wangsicong Wei on 2026-09-04.
//

#pragma once
import std;
#include "Mesh.h"
#include "Texture.h"
#include "core/Log.h"
#include "gfx/Transform.h"
#include "gfx/vulkan/VulkanDevice.h"
#include "gfx/vulkan/resource_management/GpuResourceRegistry.h"

namespace vva::gfx::vulkan {
    struct ModelLoadInfo {
        std::string mesh_path;
        std::string texture_path;
    };

    struct Model {
        Transform transform;
        MeshHandle mesh_handle;
        TextureHandle texture_handle;


        // std::vector<Transform> transforms;
        // std::vector<Mesh> meshes;
        // //std::vector<Texture2D> textures;
        //
        static auto create(Transform transform, ModelLoadInfo load_info,
                    GpuResourceRegistry& gpu_resource_registry
        ) -> Model {
            auto mesh = gpu_resource_registry.retrieveMeshHandle(load_info.mesh_path);
            auto texture = gpu_resource_registry.retrieveTextureHandle(load_info.texture_path);

            if (!mesh.has_value() || !texture.has_value()) {
                //TODO: find better way to handle error
                throw std::runtime_error("mesh or texture not found");
            }

            return {
                .transform = transform,
                .mesh_handle = mesh.value(),
                .texture_handle = texture.value()
            };
        };
    };
}
