//
// Created by Wangsicong Wei on 2026-09-10.
//

#include "RenderPass.h"

auto vva::gfx::vulkan::pickImageTransitionComfig(RenderPassUsage usage) -> RenderPassBarrierConfig {
    if (usage == RenderPassUsage::Texture) {
        return {
            .in = {
                .old_layout = vk::ImageLayout::eUndefined,
                .new_layout = vk::ImageLayout::eColorAttachmentOptimal,
                .src_access_mask = {},
                .dst_access_mask = vk::AccessFlagBits2::eColorAttachmentWrite,
                .src_stage_mask = vk::PipelineStageFlagBits2::eFragmentShader,
                .dst_stage_mask = vk::PipelineStageFlagBits2::eColorAttachmentOutput
            },
            .out = {
                .old_layout = vk::ImageLayout::eColorAttachmentOptimal,
                .new_layout = vk::ImageLayout::eShaderReadOnlyOptimal,
                .src_access_mask = vk::AccessFlagBits2::eColorAttachmentWrite,
                .dst_access_mask = vk::AccessFlagBits2::eShaderSampledRead,
                .src_stage_mask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                .dst_stage_mask = vk::PipelineStageFlagBits2::eFragmentShader
            }
        };
    }
    else if (usage == RenderPassUsage::Swapchain) {
        return {
            .in = {
                .old_layout = vk::ImageLayout::eUndefined,
                .new_layout = vk::ImageLayout::eColorAttachmentOptimal,
                .src_access_mask = {},
                .dst_access_mask = vk::AccessFlagBits2::eColorAttachmentWrite,
                .src_stage_mask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                .dst_stage_mask = vk::PipelineStageFlagBits2::eColorAttachmentOutput
            },
            .out = {
                .old_layout = vk::ImageLayout::eColorAttachmentOptimal,
                .new_layout = vk::ImageLayout::ePresentSrcKHR,
                .src_access_mask = vk::AccessFlagBits2::eColorAttachmentWrite,
                .dst_access_mask = {},
                .src_stage_mask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                .dst_stage_mask = vk::PipelineStageFlagBits2::eBottomOfPipe
            }
        };
    } return {};
}
