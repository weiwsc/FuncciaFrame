//
// Created by Wangsicong Wei on 2026-08-31.
//

#pragma once
#include "../VulkanInclude.h"
namespace vva::gfx::vulkan{
    struct ShaderStage {
        vk::ShaderStageFlagBits shader_stage;
        std::string stage_entry_point_name;
    };
    struct Shader {
        std::string name;
        std::vector<ShaderStage> shader_stages;

    };
}