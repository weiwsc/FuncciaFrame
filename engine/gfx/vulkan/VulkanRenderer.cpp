//
// Created by Wangsicong Wei on 2026-06-11.
//

#include "VulkanRenderer.h"

#include <SDL3/SDL_vulkan.h>

#include <utility>

#include "core/Log.h"
#include "types/Sampler.h"
#include "types/Texture.h"
#include "util/VulkanLogicalDeviceUtil.h"
#include "util/VulkanPhysicalDeviceUtil.h"

namespace vva::gfx::vulkan {
    namespace {
        void transition_image_layout(
            const vk::raii::CommandBuffer& command_buffer,
            const vk::Image image,
            const vk::ImageLayout old_layout,
            const vk::ImageLayout new_layout,
            const vk::AccessFlags2 src_access_mask,
            const vk::AccessFlags2 dst_access_mask,
            const vk::PipelineStageFlags2 src_stage_mask,
            const vk::PipelineStageFlags2 dst_stage_mask,
            const vk::ImageAspectFlags image_aspect_flags
        ) {
            vk::ImageMemoryBarrier2 barrier = {
                .srcStageMask = src_stage_mask,
                .srcAccessMask = src_access_mask,
                .dstStageMask = dst_stage_mask,
                .dstAccessMask = dst_access_mask,
                .oldLayout = old_layout,
                .newLayout = new_layout,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image = image,
                .subresourceRange = {
                    .aspectMask = image_aspect_flags,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1
                }
            };
            const vk::DependencyInfo dependency_info = {
                .dependencyFlags = {},
                .imageMemoryBarrierCount = 1,
                .pImageMemoryBarriers = &barrier
            };
            command_buffer.pipelineBarrier2(dependency_info);
        }
    }

    VulkanRenderer::VulkanRenderer(VulkanContext context, GlobalDescriptors descriptors)
        : context_(std::move(context)), descriptors_(std::move(descriptors)) {
    }

    auto VulkanRenderer::createVulkanRenderer(WindowInterface& window_interface,
                                              const VulkanRendererDesc& desc) -> VulkanRenderer {
        auto instance = VulkanInstance::createVulkanInstance(desc.app_name, desc.enable_validation);
        auto surface = createSurface(window_interface, instance.instance);
        auto physical_device_selection_result = util::pickPhysicalDevice(instance.instance, surface);
        auto physical_device = physical_device_selection_result.physical_device;
        auto device = util::createVulkanDevice(physical_device, physical_device_selection_result.queue_coordinates);

        auto allocator = VulkanAllocator::create(physical_device, device.logical_device, instance.instance);

        glm::ivec2 buffer_size{};
        window_interface.GetFramebufferSize(buffer_size.x, buffer_size.y);
        auto swap_chain =
            VulkanSwapChain::createSwapChain(surface, physical_device, device.logical_device, buffer_size);
        auto frame_controller = VulkanFrameController::create(device);
        auto upload_context = createVulkanUploadContext(device);
        auto shader_compiler = shader::SlangShaderCompiler::create(desc.shader_dir);

        auto texture_sampler_set = TextureSamplerDescriptorSet::create(device.logical_device);
        auto frame_scene_data_set = FrameSceneDataDescriptorSet::create(device.logical_device, allocator.get());
        GlobalDescriptors global_descriptors = {
            .texture_sampler = std::move(texture_sampler_set),
            .frame_scene_data = std::move(frame_scene_data_set)
        };
        auto graphics_pipeline = GraphicsPipeline::create(device.logical_device, physical_device,
                                                          swap_chain.surface_format, shader_compiler,
                                                          global_descriptors);
        auto samplers = createSamplers(device.logical_device);
        auto depth_resource = createDepthResources(allocator.get(), device.logical_device, physical_device,
                                                   swap_chain.extent);

        global_descriptors.texture_sampler.writeSamplers(device.logical_device, samplers);

        Camera camera{
            .transform = {},
            .projection = glm::perspective(glm::radians(45.0f),
                                           (float)swap_chain.extent.width / (float)swap_chain.extent.height,
                                           0.1f,
                                           100.0f)
        };
        // glm::perspective produces OpenGL clip space (+Y up); Vulkan NDC has +Y down.
        // Flipping Y here keeps the image upright and preserves the mesh winding, so
        // eCounterClockwise front faces + back-face culling in the pipeline stay correct.
        camera.projection[1][1] *= -1.0f;
        camera.transform.setPosition({0, 20, 5});
        camera.transform.lookAt({0, 0, 0});

        device.limits = physical_device.getProperties().limits;


        auto draw_data = std::vector<VramVector<shader::param::BasicDrawData>>{};
        for (int i = 0; i < VulkanRenderConfig::MAX_FRAME_IN_FLIGHT; ++i) {
            draw_data.push_back(
                VramVector<shader::param::BasicDrawData>::create(allocator.get() ,device.logical_device, 100));
        }

        vva_log_info("vulkan renderer created");
        return VulkanRenderer{
            {
                .instance = std::move(instance),
                .device = std::move(device),
                .allocator = std::move(allocator),
                .surface = std::move(surface),
                .swap_chain = std::move(swap_chain),
                .frame_controller = std::move(frame_controller),
                .upload_context = std::move(upload_context),
                .slang_shader_compiler = std::move(shader_compiler),
                .graphics_pipeline = std::move(graphics_pipeline),
                .samplers = std::move(samplers),
                .depth_resource = std::move(depth_resource),
                .models = {},
                .camera = camera,
                .draw_datas = std::move(draw_data)
            },
            std::move(global_descriptors)
        };
    }

    auto VulkanRenderer::loadModel(ModelLoadInfo model_load_info) -> ModelHandle {
        auto result = context_.models.createModel(std::move(model_load_info),
                                                  context_.allocator.get(),
                                                  context_.device,
                                                  context_.upload_context);
        if (result.has_value()) {
            auto handle = result.value();
            auto& texture = context_.models.textures[handle.handle];
            texture.slot = handle.handle;
            descriptors_.texture_sampler.writeTexture(context_.device.logical_device, texture.slot, *texture.image_view);
            return handle;
        }
        else {
            throw std::runtime_error{result.error()};
        }
    }

    auto VulkanRenderer::startFrame() -> void {
        //graphicsQueue.waitIdle();
        auto frame_index = context_.frame_controller.getFrameIndex();
        auto& frame_resource = context_.frame_controller.frame();
        frame_state_store_.frame_index = frame_index;
        auto fence_result = context_
                            .device
                            .logical_device.waitForFences(*frame_resource.in_flight_fences, vk::True,
                                                          UINT64_MAX);
        if (fence_result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to wait for fence!");
        }


        auto [result, imageIndex] = context_.swap_chain.handle.acquireNextImage(
            UINT64_MAX, *frame_resource.image_available_semaphore, nullptr);
        frame_state_store_.image_index = imageIndex;
        // Due to VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS being defined, eErrorOutOfDateKHR can be checked as a result
        // here and does not need to be caught by an exception.
        // if (result == vk::Result::eErrorOutOfDateKHR) {
        //     recreateSwapChain();
        //     return;
        // }
        // On other success codes than eSuccess and eSuboptimalKHR we just throw an exception.
        // On any error code, aquireNextImage already threw an exception.
        if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
            assert(result == vk::Result::eTimeout || result == vk::Result::eNotReady);
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        descriptors_.frame_scene_data.writeFrameUniform(
            updateFrameData(),
            frame_index
        );
        // Only reset the fence if we are submitting work
        context_.device.logical_device.resetFences(*frame_resource.in_flight_fences);
        frame_resource.command_buffer.reset();


        auto& command_buffer = frame_resource.command_buffer;
        // Before starting rendering, transition the swapchain image to COLOR_ATTACHMENT_OPTIMAL
        command_buffer.begin({});
        transition_image_layout(
            command_buffer,
            context_.swap_chain.images[imageIndex].image,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eColorAttachmentOptimal,
            {}, // srcAccessMask (no need to wait for previous operations)
            vk::AccessFlagBits2::eColorAttachmentWrite, // dstAccessMask
            vk::PipelineStageFlagBits2::eColorAttachmentOutput, // srcStage
            vk::PipelineStageFlagBits2::eColorAttachmentOutput, // dstStage
            vk::ImageAspectFlagBits::eColor
        );

        transition_image_layout(
            command_buffer,
            context_.depth_resource.source_image.handle(),
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eDepthAttachmentOptimal,
            vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
            vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
            vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
            vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
            vk::ImageAspectFlagBits::eDepth);

        vk::ClearValue clear_color = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
        vk::ClearValue clear_depth = vk::ClearDepthStencilValue(1.0f, 0);
        vk::RenderingAttachmentInfo attachment_info = {
            .imageView = context_.swap_chain.images[imageIndex].image_view,
            .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .loadOp = vk::AttachmentLoadOp::eClear,
            .storeOp = vk::AttachmentStoreOp::eStore,
            .clearValue = clear_color
        };

        vk::RenderingAttachmentInfo depth_attachment_info = {
            .imageView = context_.depth_resource.image_view,
            .imageLayout = vk::ImageLayout::eDepthAttachmentOptimal,
            .loadOp = vk::AttachmentLoadOp::eClear,
            .storeOp = vk::AttachmentStoreOp::eDontCare,
            .clearValue = clear_depth
        };
        auto extent = context_.swap_chain.extent;
        vk::RenderingInfo rendering_info = {
            .renderArea = {.offset = {0, 0}, .extent = extent},
            .layerCount = 1,
            .colorAttachmentCount = 1,
            .pColorAttachments = &attachment_info,
            .pDepthAttachment = &depth_attachment_info
        };

        command_buffer.beginRendering(rendering_info);

        command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, context_.graphics_pipeline.handle);

        command_buffer.setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(extent.width),
                                                   static_cast<float>(extent.height), 0.0f, 1.0f));
        command_buffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), extent));
    }

    auto VulkanRenderer::endFrame() -> void {

        auto frame_index = context_.frame_controller.getFrameIndex();
        auto& frame_resource = context_.frame_controller.frame();
        auto& command_buffer = frame_resource.command_buffer;
        command_buffer.endRendering();
        transition_image_layout(
            command_buffer,
            context_.swap_chain.images[frame_state_store_.image_index].image,
            vk::ImageLayout::eColorAttachmentOptimal,
            vk::ImageLayout::ePresentSrcKHR,
            vk::AccessFlagBits2::eColorAttachmentWrite, // srcAccessMask
            {}, // dstAccessMask
            vk::PipelineStageFlagBits2::eColorAttachmentOutput, // srcStage
            vk::PipelineStageFlagBits2::eBottomOfPipe, // dstStage
            vk::ImageAspectFlagBits::eColor
        );
        command_buffer.end();

        const auto& render_finished_semaphore = context_.swap_chain.images[frame_state_store_.image_index].render_finished_semaphore;

        vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
        const vk::SubmitInfo submitInfo{
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &*frame_resource.image_available_semaphore,
            .pWaitDstStageMask = &waitDestinationStageMask,
            .commandBufferCount = 1,
            .pCommandBuffers = &*command_buffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &*render_finished_semaphore
        };
        context_.device.queues.graphics_queue.submit(submitInfo, *frame_resource.in_flight_fences);
        // auto result = context_.device.logical_device.waitForFences(*frame_resource.in_flight_fences, vk::True, UINT64_MAX);
        //
        // if (result != vk::Result::eSuccess) {
        //     throw std::runtime_error("failed to wait for fence!");
        // }

        const vk::PresentInfoKHR presentInfoKHR{
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &*render_finished_semaphore,
            .swapchainCount = 1,
            .pSwapchains = &*context_.swap_chain.handle,
            .pImageIndices = &frame_state_store_.image_index
        };
        auto result = context_.device.queues.present_queue.presentKHR(presentInfoKHR);

        // Due to VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS being defined, eErrorOutOfDateKHR can be checked as a result
        // here and does not need to be caught by an exception.
        // if ((result == vk::Result::eSuboptimalKHR) || (result == vk::Result::eErrorOutOfDateKHR) ||
        //     framebufferResized) {
        //     framebufferResized = false;
        //     recreateSwapChain();
        //     } else {
        //         // There are no other success codes than eSuccess; on any error code, presentKHR already threw an exception.
        //         assert(result == vk::Result::eSuccess);
        //     }
        context_.frame_controller.advanceFrame();
        frame_state_store_ = {};
    }

    auto VulkanRenderer::drawModel(ModelHandle model_handle) -> void {
        auto&  command_buffer = context_.frame_controller.frame().command_buffer;
        auto& mesh = context_.models.meshes[model_handle.handle];
        auto& model_transform = context_.models.transforms[model_handle.handle];
        auto& model_texture = context_.models.textures[model_handle.handle];
        command_buffer.bindVertexBuffers(0, (mesh.vertex_buffer.handle()), {0});
        command_buffer.bindIndexBuffer(mesh.index_buffer.handle(), 0, vk::IndexType::eUint32);

        std::array sets = {
            *descriptors_.texture_sampler.set,
            *descriptors_.frame_scene_data.sets[frame_state_store_.frame_index]
        };
        command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
            context_.graphics_pipeline.layout,
            0,
            sets,
            nullptr
        );
        auto data = std::vector<shader::param::BasicDrawData>{};
        for (int i = 0; i < 50; ++i) {
            Transform transform = model_transform;
            transform.translate({i * 2, 0,0});
            data.push_back(shader::param::BasicDrawData{
            .model_matrix = transform.getModelMatrix(),   // identity is fine for the first look
            .texture_index = model_texture.slot,
            .sampler_index = SAMPLER_LINEAR_REPEAT});
        }
        auto& vram_vec = context_.draw_datas[frame_state_store_.frame_index];
        vram_vec.clear();
        vram_vec.insert_range(data);
        vram_vec.flush();
        const shader::param::PushConstants pc {
            .instances = vram_vec.deviceAddress(),
            .first_instance = 0
        };
        command_buffer.pushConstants<shader::param::PushConstants>(
            *context_.graphics_pipeline.layout, vk::ShaderStageFlagBits::eAll, 0, pc);

        command_buffer.drawIndexed(mesh.index_count, vram_vec.size(), 0, 0, 0);
    }

    auto VulkanRenderer::createSurface(WindowInterface& window,
                                       const vk::raii::Instance& instance) -> vk::raii::SurfaceKHR {
        VkSurfaceKHR surface;
        if (!SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(window.GetNativeWindow()), *instance, nullptr,
                                      &surface)) {
            throw std::runtime_error("failed to create window surface!");
        }
        return vk::raii::SurfaceKHR(instance, surface);
    }

    auto VulkanRenderer::updateFrameData() -> shader::param::FrameUniformBuffer {
        auto& camera = context_.camera;

        static auto start_time = std::chrono::high_resolution_clock::now();
        auto current_time = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
        float delta_time = time - last_time_;
        last_time_ = time;
        glm::vec2 resolution = {context_.swap_chain.extent.width, context_.swap_chain.extent.height};
        return {
            .view = camera.lookingAt(),
            .projection = camera.projection,
            .view_projection = camera.projection * camera.lookingAt(),
            .inverse_view = camera.transform.getModelMatrix(),
            .inverse_projection = glm::inverse(camera.projection),
            .camera_position = camera.transform.position(),
            .resolution = resolution,
            .mouse = {0, 0},
            .time = last_time_,
            .delta_time = delta_time,
            .lights = 0
        };
    }
}
