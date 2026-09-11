//
// Created by Wangsicong Wei on 2026-08-30.
//

#include "GraphicsPipeline.h"

#include "resource_management/AllocatedImage.h"
#include "VulkanConfig.h"
#include "core/Log.h"
#include "types/Param.h"
#include "types/Sampler.h"
#include "types/Texture.h"

namespace vva::gfx::vulkan {
    struct PipelineShaderModule {
        vk::raii::ShaderModule shader_module;
        vk::PipelineShaderStageCreateInfo pipeline_shader_stage_create_info;
    };

    namespace {
        auto pipelineShaderModuleInfo(const SlangShaderCompiler& compiler, const vk::raii::Device& device,
                                      const vk::ShaderStageFlagBits stage_flag_bits, const std::string_view module_name,
                                      const std::string_view entry_point) -> PipelineShaderModule {
            auto shader_code = compiler.compile(module_name.data(), entry_point.data());
            vk::ShaderModuleCreateInfo vk_shader_module_create_info{
                .codeSize = shader_code.size() * sizeof(uint32_t),
                .pCode = shader_code.data()
            };
            auto shader_module = vk::raii::ShaderModule{device, vk_shader_module_create_info};
            auto pipeline_shader_stage_create_info = vk::PipelineShaderStageCreateInfo{
                .stage = stage_flag_bits,
                .module = shader_module,
                .pName = "main"
            };
            return {
                .shader_module = std::move(shader_module),
                .pipeline_shader_stage_create_info = pipeline_shader_stage_create_info
            };
        }

        auto findSupportedFormat(const std::vector<vk::Format>& candidates,
                                 vk::ImageTiling tiling,
                                 vk::FormatFeatureFlags features,
                                 const vk::raii::PhysicalDevice& physical_device) -> vk::Format {
            for (const auto format : candidates) {
                vk::FormatProperties props = physical_device.getFormatProperties(format);

                if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
                    return format;
                }
                if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
                    return format;
                }
            }
            throw std::runtime_error("failed to find supported format!");
        }
    }

    auto findDepthFormat(const vk::raii::PhysicalDevice& physical_device) -> vk::Format {
        return findSupportedFormat(
            {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eDepthStencilAttachment,
            physical_device
        );
    }

    auto createDepthResources(const VmaAllocator& allocator, const vk::raii::Device& device,
                              const vk::raii::PhysicalDevice& physical_device, vk::Extent2D size) -> Texture2D {
        vk::Format depth_format = findDepthFormat(physical_device);
        vk::ImageCreateInfo image_create_info{
            .imageType = vk::ImageType::e2D,
            .format = depth_format,
            .extent = {size.width, size.height, 1},
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = vk::SampleCountFlagBits::e1,
            .tiling = vk::ImageTiling::eOptimal,
            .usage = vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eSampled,
            .sharingMode = vk::SharingMode::eExclusive
        };
        return createTextureImage(allocator, device, image_create_info, depth_format, vk::ImageAspectFlagBits::eDepth);
    }

    auto createIntermediateResource(const VmaAllocator& allocator,
                                    const vk::raii::Device& device,
                                    const vk::raii::PhysicalDevice& physical_device, vk::Extent2D size) -> Texture2D {
        vk::ImageCreateInfo image_create_info{
            .imageType = vk::ImageType::e2D,
            .format = vk::Format::eR16G16B16A16Sfloat,
            .extent = {size.width, size.height, 1},
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = vk::SampleCountFlagBits::e1,
            .tiling = vk::ImageTiling::eOptimal,
            .usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            .sharingMode = vk::SharingMode::eExclusive
        };
        return createTextureImage(allocator,
                                  device, image_create_info,
                                  vk::Format::eR16G16B16A16Sfloat,
                                  vk::ImageAspectFlagBits::eColor);
    }

    auto GraphicsPipeline::create(std::string shader_name,
                                  const vk::raii::Device& device,
                                  const vk::raii::PhysicalDevice& physical_device,
                                  const vk::SurfaceFormatKHR& surface_format,
                                  SlangShaderCompiler& compiler,
                                  const GlobalDescriptors& global_descriptors,
                                  bool has_depth_attachment,
                                  vk::CullModeFlagBits cull_mode) -> GraphicsPipeline {
        auto vertex_stage = pipelineShaderModuleInfo(compiler, device, vk::ShaderStageFlagBits::eVertex, shader_name,
                                                     "vertMain");
        auto fragment_stage = pipelineShaderModuleInfo(compiler, device, vk::ShaderStageFlagBits::eFragment,
                                                       shader_name,
                                                       "fragMain");
        vk::PipelineShaderStageCreateInfo shader_stages[] = {
            vertex_stage.pipeline_shader_stage_create_info,
            fragment_stage.pipeline_shader_stage_create_info
        };

        //get the description from the vertex type
        auto binding_description = shader::param::PosNormalUV::getBindingDescription();
        auto attribute_descriptions = shader::param::PosNormalUV::getAttributeDescriptions();
        //this step is binding the shader vertext input layout
        vk::PipelineVertexInputStateCreateInfo vertex_input_info{
            .vertexBindingDescriptionCount = 1,
            //binding number(slot?), size of the vertex input, the rate it is consumed
            .pVertexBindingDescriptions = &binding_description,
            //binding slot, and for each attribute, what's the location and length
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size()),
            .pVertexAttributeDescriptions = attribute_descriptions.data()
        };

        //this specify how should the vertex data be interpreted,
        //eTriangleList: each 3 vertex is an independent triangle
        //eTriangleStrip: a ribbon of triangles ...
        //the redraw usually only matters for drawing terrain or water
        //that need to break the strip and restart at new line
        vk::PipelineInputAssemblyStateCreateInfo input_assembly{
            .topology = vk::PrimitiveTopology::eTriangleList
        };

        ///this can store the viewport size, but with dynamic rendering
        ///we do not bake the viewport size into the pipeline
        /// for vr, there would be 2 viewports and the scissor count need to be the same as viewport count
        vk::PipelineViewportStateCreateInfo viewport_state{
            .viewportCount = 1,
            .scissorCount = 1
        };

        //I don't know what is depthBias
        vk::PipelineRasterizationStateCreateInfo rasterizer{
            .depthClampEnable = vk::False,
            .rasterizerDiscardEnable = vk::False,
            .polygonMode = vk::PolygonMode::eFill,
            .cullMode = cull_mode,
            .frontFace = vk::FrontFace::eCounterClockwise,
            .depthBiasEnable = vk::False,
            .depthBiasSlopeFactor = 1.0f, .lineWidth = 1.0f
        };
        //for MSAA the samples would be e4, plus other settings
        vk::PipelineMultisampleStateCreateInfo multisampling{
            .rasterizationSamples = vk::SampleCountFlagBits::e1,
            .sampleShadingEnable = vk::False
        };
        ///seems like this can do similar stuff as csp layer mode (add, multiple etc), need to verify later
        /// seems like some of layer blending in csp cannot be directly achieved by this, when it need to read the designation image
        ///        -> techniques that apparently solve this:
        ///              - Ping-pong,
        ///              - VK_EXT_blend_operation_advanced,
        ///              - Framebuffer fetch / VK_EXT_rasterization_order_attachment_access
        vk::PipelineColorBlendAttachmentState color_blend_attachment{
            .blendEnable = vk::False, //disable is for opaque stuff, enable if there's transparent stuff
            .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        };
        ///if there's multiple image in a draw (like G-buffer), use an array of
        ///PipelineMultisampleStateCreateInfo, the order of it decide which slot is it for
        vk::PipelineColorBlendStateCreateInfo color_blending{
            .logicOpEnable = vk::False,
            .logicOp = vk::LogicOp::eCopy, //does this even do anything when logicOpEnable is false?
            .attachmentCount = 1, //array.size() when there's multiple
            .pAttachments = &color_blend_attachment // <- in that case put the array here
        };

        //what is not baked into the pso and instead set during command recording
        //here, the viewport and scissor is dynamic, so resizing the window doesn't require
        //recreating the graphics pipeline
        std::vector dynamic_states = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
        };
        vk::PipelineDynamicStateCreateInfo dynamic_state{
            .dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
            .pDynamicStates = dynamic_states.data()
        };
        //create descriptor set, change this later
        std::vector<vk::DescriptorSetLayout> descriptor_set_layouts;
        descriptor_set_layouts.push_back(*global_descriptors.texture_sampler.layout);
        descriptor_set_layouts.push_back(*global_descriptors.frame_scene_data.layout);

        vk::PushConstantRange push_constant_range{
            .stageFlags = vk::ShaderStageFlagBits::eAll,
            .offset = 0,
            .size = sizeof(shader::param::PushConstants)
        };

        vk::PipelineLayoutCreateInfo pipeline_layout_info{
            .setLayoutCount = static_cast<uint32_t>(descriptor_set_layouts.size()),
            .pSetLayouts = descriptor_set_layouts.data(),
            .pushConstantRangeCount = 1,
            .pPushConstantRanges = &push_constant_range //fill this when adding push constant
        };

        auto pipeline_layout = vk::raii::PipelineLayout(device, pipeline_layout_info);


        vk::PipelineDepthStencilStateCreateInfo depth_stencil{};
        vk::Format depth_format{};

        if (has_depth_attachment) {
            depth_stencil = {
                .depthTestEnable = vk::True,
                .depthWriteEnable = vk::True,
                .depthCompareOp = vk::CompareOp::eLess,
                .depthBoundsTestEnable = vk::False,
                .stencilTestEnable = vk::False
            };
            depth_format = findDepthFormat(physical_device);
        }
        vk::GraphicsPipelineCreateInfo graphics_pipeline_create_info{
            .stageCount = 2,
            .pStages = shader_stages,
            .pVertexInputState = &vertex_input_info,
            .pInputAssemblyState = &input_assembly,
            .pViewportState = &viewport_state,
            .pRasterizationState = &rasterizer,
            .pMultisampleState = &multisampling,
            .pColorBlendState = &color_blending,
            .pDynamicState = &dynamic_state,
            .layout = pipeline_layout,
            .renderPass = nullptr
        };
        vk::PipelineRenderingCreateInfo pipeline_rendering_create_info{
            .colorAttachmentCount = 1, //prob need to change this too if there's multiple image
            .pColorAttachmentFormats = &surface_format.format
        };
        if (has_depth_attachment) {
            graphics_pipeline_create_info.pDepthStencilState = &depth_stencil;
            pipeline_rendering_create_info.depthAttachmentFormat = depth_format;
        }
        vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipeline_create_info_chain =
        {
            graphics_pipeline_create_info,
            pipeline_rendering_create_info
        };

        auto graphics_pipeline = vk::raii::Pipeline(device, nullptr,
                                                    pipeline_create_info_chain.get<vk::GraphicsPipelineCreateInfo>());
        vva_log_info("graphics pipeline created from shader \"{}\"", shader_name);
        auto samplers = createSamplers(device);

        return {
            .layout = std::move(pipeline_layout),
            .handle = std::move(graphics_pipeline),
        };
    }
}
