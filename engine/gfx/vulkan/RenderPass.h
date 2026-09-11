//
// Created by Wangsicong Wei on 2026-09-10.
//

#pragma once
#include "GraphicsPipeline.h"
#include "VulkanImage.h"
#include "resource_management/GpuResourceRegistry.h"
#include "types/Texture.h"

namespace vva::gfx::vulkan {
    enum class RenderPassUsage {
        Swapchain,
        Texture
    };

    struct RenderPassBarrierConfig {
        BarrierImageTransitionConfig in;
        BarrierImageTransitionConfig out;
    };

    auto pickImageTransitionComfig(RenderPassUsage usage) -> RenderPassBarrierConfig ;

    struct RenderPass {
        const vk::raii::CommandBuffer& command_buffer;
        const GraphicsPipeline& graphics_pipeline;
        vk::Image render_target_image;
        vk::ImageView render_target_image_view;
        vk::Extent2D extent;
        TextureHandle depth_image;
        RenderPassBarrierConfig barrier_config;
        vk::ClearValue clear_color_value;
        vk::ClearValue clear_depth_stencil_value;

        auto begin() -> void {

            transition_image_layout(
                command_buffer,
                render_target_image,
                barrier_config.in,
                vk::ImageAspectFlagBits::eColor
            );
            vk::RenderingAttachmentInfo attachment_info = {
                .imageView = render_target_image_view,
                .imageLayout = barrier_config.in.new_layout,
                .loadOp = vk::AttachmentLoadOp::eClear,
                .storeOp = vk::AttachmentStoreOp::eStore,
                .clearValue = clear_color_value
            };

            vk::RenderingInfo rendering_info = {
                .renderArea = {.offset = {0, 0}, .extent = extent},
                .layerCount = 1,
                .colorAttachmentCount = 1,
                .pColorAttachments = &attachment_info,
            };
            vk::RenderingAttachmentInfo depth_attachment_info{};
            if (depth_image.exists) {
                const auto& depth_target = depth_image.getTexture();
                transition_image_layout(
                    command_buffer,
                    depth_target.source_image.handle(),
                    {
                    vk::ImageLayout::eUndefined,
                    vk::ImageLayout::eDepthAttachmentOptimal,
                    vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
                    vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
                    vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
                    vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
                        },
                    vk::ImageAspectFlagBits::eDepth);

                depth_attachment_info = {
                    .imageView = depth_target.image_view,
                    .imageLayout = vk::ImageLayout::eDepthAttachmentOptimal,
                    .loadOp = vk::AttachmentLoadOp::eClear,
                    .storeOp = vk::AttachmentStoreOp::eDontCare,
                    .clearValue = clear_depth_stencil_value
                };
                rendering_info.pDepthAttachment = &depth_attachment_info;
            }

            command_buffer.beginRendering(rendering_info);

            command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphics_pipeline.handle);

            command_buffer.setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(extent.width),
                                                       static_cast<float>(extent.height), 0.0f, 1.0f));
            command_buffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), extent));
        }

        auto end() -> void {
            command_buffer.endRendering();
            transition_image_layout(
                command_buffer,
                render_target_image,
                barrier_config.out,
                vk::ImageAspectFlagBits::eColor
            );
        }
    };
}
