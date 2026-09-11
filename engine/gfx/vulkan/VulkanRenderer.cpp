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
    VulkanRenderer::VulkanRenderer(VulkanContext context, GlobalDescriptors descriptors)
        : context_(std::move(context)), descriptors_(std::move(descriptors)),
          gpu_resource_registry_(context_.device, descriptors_.texture_sampler, context_.allocator,
                                 context_.upload_context) {
        auto intermediate_resource = createIntermediateResource(context_.allocator.get(),
                                                                context_.device.logical_device,
                                                                context_.device.physical_device,
                                                                context_.swap_chain.extent);
        context_.intermediate_resource = gpu_resource_registry_.registerTexture(std::move(intermediate_resource));
        auto depth_resource = createDepthResources(context_.allocator.get(),
                                                   context_.device.logical_device,
                                                   context_.device.physical_device,
                                                   context_.swap_chain.extent);
        context_.depth_resource = gpu_resource_registry_.registerTexture(std::move(depth_resource));
    }

    auto VulkanRenderer::createVulkanRenderer(WindowInterface& window_interface,
                                              const VulkanRendererDesc& desc) -> VulkanRenderer {
        auto instance =
            VulkanInstance::createVulkanInstance(desc.app_name, VulkanSetupConfig::ENABLE_VALIDATION_LAYERS);
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
        auto upload_context = VulkanUploadContext::create(device);
        auto shader_compiler = shader::SlangShaderCompiler::create(desc.shader_dir);

        auto texture_sampler_set = TextureSamplerDescriptorSet::create(device.logical_device);
        auto frame_scene_data_set = FrameSceneDataDescriptorSet::create(device.logical_device, allocator.get());
        GlobalDescriptors global_descriptors = {
            .texture_sampler = std::move(texture_sampler_set),
            .frame_scene_data = std::move(frame_scene_data_set)
        };
        auto graphics_pipeline_3d = GraphicsPipeline::create("my_shader",
                                                             device.logical_device, physical_device,
                                                             {
                                                                 .format = vk::Format::eR16G16B16A16Sfloat,
                                                                 .colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear
                                                             }, shader_compiler,
                                                             global_descriptors, true);
        auto graphics_pipeline_2d = GraphicsPipeline::create("my_2d_shader", device.logical_device, physical_device,
                                                             {
                                                                 .format = vk::Format::eR16G16B16A16Sfloat,
                                                                 .colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear
                                                             }, shader_compiler,
                                                             global_descriptors, false,
                                                             vk::CullModeFlagBits::eNone);
        auto graphics_pipeline_swap_buffer = GraphicsPipeline::create("my_2d_shader", device.logical_device,
                                                                      physical_device,
                                                                      swap_chain.surface_format, shader_compiler,
                                                                      global_descriptors, false,
                                                                      vk::CullModeFlagBits::eNone);
        auto samplers = createSamplers(device.logical_device);


        global_descriptors.texture_sampler.writeSamplers(device.logical_device, samplers);


        device.limits = physical_device.getProperties().limits;


        auto draw_data = std::vector<VramVector<shader::param::BasicDrawData>>{};
        for (int i = 0; i < VulkanRenderConfig::MAX_FRAME_IN_FLIGHT; ++i) {
            draw_data.push_back(
                VramVector<shader::param::BasicDrawData>::create(allocator.get(), device.logical_device, 1000));
        }
        auto texture_draw_data = std::vector<VramVector<shader::param::TextureDrawData>>{};
        for (int i = 0; i < VulkanRenderConfig::MAX_FRAME_IN_FLIGHT; ++i) {
            texture_draw_data.push_back(
                VramVector<shader::param::TextureDrawData>::create(
                    allocator.get(), device.logical_device, 1000));
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
                .graphics_pipeline_2d = std::move(graphics_pipeline_2d),
                .graphics_pipeline_3d = std::move(graphics_pipeline_3d),
                .graphics_pipeline_swap_buffer = std::move(graphics_pipeline_swap_buffer),
                .samplers = std::move(samplers),
                .draw_datas = std::move(draw_data),
                .texture_draw_infos = std::move(texture_draw_data)
            },
            std::move(global_descriptors)
        };
    }


    auto VulkanRenderer::startFrame(TimeInfo time_info) -> FrameState {
        return context_.frame_controller.startFrame(time_info, context_.device, context_.swap_chain);
    }

    auto VulkanRenderer::endFrame(FrameState frame_state) -> void {
        RenderPass swap_buffer_pass{
            .command_buffer = frame_state.frame_resource.command_buffer,
            .graphics_pipeline = context_.graphics_pipeline_swap_buffer,
            .render_target_image = context_.swap_chain.images[frame_state.image_index].image,
            .render_target_image_view = context_.swap_chain.images[frame_state.image_index].image_view,
            .extent = context_.swap_chain.extent,
            .depth_image = {nullptr, 0, false},
            .barrier_config = pickImageTransitionComfig(RenderPassUsage::Swapchain),
            .clear_color_value = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f),
            .clear_depth_stencil_value = vk::ClearDepthStencilValue(1.0f, 0.0f)
        };
        swap_buffer_pass.begin();

        auto& texture = context_.intermediate_resource.getTexture();
        glm::vec2 size = {texture.extent.width, texture.extent.height};
        std::array textures{
            shader::param::TextureDrawData{
                .texture_index = texture.slot,
                .sampler_index = SAMPLER_LINEAR_CLAMP,
                .size = size
            }
        };
        //=========================refactor this part
        std::array sets = {
            *descriptors_.texture_sampler.set,
            *descriptors_.frame_scene_data.sets[frame_state.frame_index]
        };
        auto& command_buffer = frame_state.frame_resource.command_buffer;
        command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                          context_.graphics_pipeline_swap_buffer.layout,
                                          0,
                                          sets,
                                          nullptr
        );
        auto& draw_data = context_.texture_draw_infos[frame_state.frame_index];
        //draw_data.clear();
        draw_data.insert_range(textures);
        draw_data.flush();

        const shader::param::PushConstants pc{
            .instances = draw_data.deviceAddressAt(draw_data.size() - textures.size()),
        };
        command_buffer.pushConstants<shader::param::PushConstants>(
            *context_.graphics_pipeline_swap_buffer.layout, vk::ShaderStageFlagBits::eAll, 0, pc);

        command_buffer.draw(3, textures.size(), 0, 0);
        //=======================================
        swap_buffer_pass.end();
        context_.frame_controller.endFrame(frame_state, context_.device, context_.swap_chain);
    }

    auto VulkanRenderer::get3dRenderPass(FrameState frame_state) -> RenderPass {
        auto& image = context_.intermediate_resource.getTexture();
        return {
            .command_buffer = frame_state.frame_resource.command_buffer,
            .graphics_pipeline = context_.graphics_pipeline_3d,
            .render_target_image = image.source_image.handle(),
            .render_target_image_view = image.image_view,
            .extent = image.extent,
            .depth_image = context_.depth_resource,
            .barrier_config = pickImageTransitionComfig(RenderPassUsage::Texture),
            .clear_color_value = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f),
            .clear_depth_stencil_value = vk::ClearDepthStencilValue(1.0f, 0.0f)
        };
    }

    auto VulkanRenderer::get2dRenderPass(FrameState frame_state) -> RenderPass {
        auto& image = context_.intermediate_resource.getTexture();
        return {
            .command_buffer = frame_state.frame_resource.command_buffer,
            .graphics_pipeline = context_.graphics_pipeline_2d,
            .render_target_image = image.source_image.handle(),
            .render_target_image_view = image.image_view,
            .extent = image.extent,
            .depth_image = {nullptr, 0, false},
            .barrier_config = pickImageTransitionComfig(RenderPassUsage::Texture),
            .clear_color_value = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f),
            .clear_depth_stencil_value = vk::ClearDepthStencilValue(1.0f, 0.0f)
        };
    }

    auto VulkanRenderer::drawModel(std::span<const Model> models, FrameState frame_state) -> void {
        auto& command_buffer = frame_state.frame_resource.command_buffer;
        auto& mesh = gpu_resource_registry_.meshes[models.front().mesh_handle.id];
        auto& model_texture = gpu_resource_registry_.textures[models.front().texture_handle.id];
        command_buffer.bindVertexBuffers(0, (mesh.vertex_buffer.handle()), {0});
        command_buffer.bindIndexBuffer(mesh.index_buffer.handle(), 0, vk::IndexType::eUint32);

        std::array sets = {
            *descriptors_.texture_sampler.set,
            *descriptors_.frame_scene_data.sets[frame_state.frame_index]
        };
        command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                          context_.graphics_pipeline_3d.layout,
                                          0,
                                          sets,
                                          nullptr
        );
        auto data = std::vector<shader::param::BasicDrawData>{};
        for (auto& model : models) {
            data.push_back(shader::param::BasicDrawData{
                .model_matrix = model.transform.getModelMatrix(), // identity is fine for the first look
                .texture_index = model_texture.slot,
                .sampler_index = SAMPLER_LINEAR_REPEAT
            });
        }
        auto& vram_vec = context_.draw_datas[frame_state.frame_index];
        vram_vec.clear();
        vram_vec.insert_range(data);
        vram_vec.flush();
        const shader::param::PushConstants pc{
            .instances = vram_vec.deviceAddress(),
        };
        command_buffer.pushConstants<shader::param::PushConstants>(
            *context_.graphics_pipeline_3d.layout, vk::ShaderStageFlagBits::eAll, 0, pc);

        command_buffer.drawIndexed(mesh.index_count, vram_vec.size(), 0, 0, 0);
    }

    auto VulkanRenderer::drawTexture(std::span<shader::param::TextureDrawData> textures,
                                     FrameState frame_state) -> void {
        auto& command_buffer = frame_state.frame_resource.command_buffer;
        std::array sets = {
            *descriptors_.texture_sampler.set,
            *descriptors_.frame_scene_data.sets[frame_state.frame_index]
        };
        command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                          context_.graphics_pipeline_2d.layout,
                                          0,
                                          sets,
                                          nullptr
        );
        auto& draw_data = context_.texture_draw_infos[frame_state.frame_index];
        draw_data.clear();
        draw_data.insert_range(textures);
        draw_data.flush();

        const shader::param::PushConstants pc{
            .instances = draw_data.deviceAddressAt(draw_data.size() - textures.size()),
        };
        command_buffer.pushConstants<shader::param::PushConstants>(
            *context_.graphics_pipeline_2d.layout, vk::ShaderStageFlagBits::eAll, 0, pc);

        command_buffer.draw(3, textures.size(), 0, 0);
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

    auto VulkanRenderer::updateSceneData(const Camera& camera,
                                         glm::vec2 mouse_pos,
                                         const FrameState& frame_state) const -> void {
        glm::vec2 resolution = {context_.swap_chain.extent.width, context_.swap_chain.extent.height};
        auto frame_data = shader::param::FrameUniformBuffer{
            .view = camera.lookingAt(),
            .projection = camera.projection,
            .view_projection = camera.projection * camera.lookingAt(),
            .inverse_view = camera.transform.getModelMatrix(),
            .inverse_projection = glm::inverse(camera.projection),
            .camera_position = camera.transform.position(),
            .resolution = resolution,
            .mouse = mouse_pos,
            .time = last_time_,
            .delta_time = frame_state.time_info.delta_time,
            .lights = 0
        };
        descriptors_.frame_scene_data.writeFrameUniform(frame_data, frame_state.frame_index);
    }
}
