//
// Created by Wangsicong Wei on 2026-09-03.
//

#pragma once
#include "FrameSceneDataDescriptorSet.h"
#include "TextureSamplerDescriptorSet.h"
#include "../VulkanConfig.h"
#include "../VulkanInclude.h"


namespace vva::gfx::vulkan {
    struct GlobalDescriptors {
        TextureSamplerDescriptorSet texture_sampler;
        FrameSceneDataDescriptorSet frame_scene_data;
    };
}
