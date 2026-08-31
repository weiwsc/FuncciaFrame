//
// Created by Wangsicong Wei on 2026-08-30.
//

#include "GraphicsPipeline.h"

#include "types/Param.h"

namespace Funccia::Graphic::Vulkan {
    struct PipelineShaderModule {
        vk::raii::ShaderModule shader_module;
        vk::PipelineShaderStageCreateInfo pipeline_shader_stage_create_info;
    };

    namespace {
        auto PipelineShaderModuleInfo(SlangShaderCompiler& compiler, const vk::raii::Device& device,
                                      vk::ShaderStageFlagBits stage_flag_bits, std::string_view module_name,
                                      std::string_view entry_point) -> PipelineShaderModule {
            auto shader_code = compiler.compile(module_name.data(), entry_point.data());
            vk::ShaderModuleCreateInfo vk_shader_module_create_info{
                .codeSize = shader_code.size() * sizeof(uint32_t),
                .pCode = shader_code.data()
            };
            auto shader_module = vk::raii::ShaderModule{device, vk_shader_module_create_info};
            auto pipeline_shader_stage_create_info = vk::PipelineShaderStageCreateInfo {
                    .stage = stage_flag_bits,
                    .module = shader_module,
                    .pName = "main"
                };
            return {
                .shader_module = std::move(shader_module),
                .pipeline_shader_stage_create_info = pipeline_shader_stage_create_info
            };
        }

        vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates,
                                       vk::ImageTiling tiling,
                                       vk::FormatFeatureFlags features,
                                       const vk::raii::PhysicalDevice& physical_device) {
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

        vk::Format findDepthFormat(const vk::raii::PhysicalDevice& physical_device) {
            return findSupportedFormat(
                {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
                vk::ImageTiling::eOptimal,
                vk::FormatFeatureFlagBits::eDepthStencilAttachment,
                physical_device
            );
        }

        auto createDescriptorSetLayout(const vk::raii::Device& device) -> vk::raii::DescriptorSetLayout {
            std::array bindings = {
                vk::DescriptorSetLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1,
                                               vk::ShaderStageFlagBits::eVertex,
                                               nullptr),
                vk::DescriptorSetLayoutBinding(1, vk::DescriptorType::eCombinedImageSampler, 1,
                                               vk::ShaderStageFlagBits::eFragment, nullptr)
            };

            vk::DescriptorSetLayoutCreateInfo layoutInfo{
                .bindingCount = static_cast<uint32_t>(bindings.size()), .pBindings = bindings.data()
            };

            return vk::raii::DescriptorSetLayout(device, layoutInfo);
        }
    }

    auto GraphicsPipeline::Create(const vk::raii::Device& device,
                                  const vk::raii::PhysicalDevice& physical_device,
                                  const vk::SurfaceFormatKHR& surface_format,
                                  SlangShaderCompiler& compiler) -> GraphicsPipeline {
        auto vertex_stage = PipelineShaderModuleInfo(compiler, device, vk::ShaderStageFlagBits::eVertex, "my_shader",
                                                     "vertex_main");
        auto fragment_stage = PipelineShaderModuleInfo(compiler, device, vk::ShaderStageFlagBits::eFragment,
                                                       "my_shader",
                                                       "fragment_main");
        vk::PipelineShaderStageCreateInfo shader_stages[] = {
            vertex_stage.pipeline_shader_stage_create_info,
            fragment_stage.pipeline_shader_stage_create_info
        };

        //get the description from the vertex type
        auto bindingDescription = Shader::Param::PosNormalUV::getBindingDescription();
        auto attributeDescriptions = Shader::Param::PosNormalUV::getAttributeDescriptions();
        //this step is binding the shader vertext input layout
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
            .vertexBindingDescriptionCount = 1,
            //binding number(slot?), size of the vertex input, the rate it is consumed
            .pVertexBindingDescriptions = &bindingDescription,
            //binding slot, and for each attribute, what's the location and length
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
            .pVertexAttributeDescriptions = attributeDescriptions.data()
        };

        //this specify how should the vertex data be interpreted,
        //eTriangleList: each 3 vertex is an independent triangle
        //eTriangleStrip: a ribbon of triangles ...
        //the redraw usually only matters for drawing terrain or water
        //that need to break the strip and restart at new line
        vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
            .topology = vk::PrimitiveTopology::eTriangleList
        };

        ///this can store the viewport size, but with dynamic rendering
        ///we do not bake the viewport size into the pipeline
        /// for vr, there would be 2 viewports and the scissor count need to be the same as viewport count
        vk::PipelineViewportStateCreateInfo viewportState{
            .viewportCount = 1,
            .scissorCount = 1
        };

        //I don't know what is depthBias
        vk::PipelineRasterizationStateCreateInfo rasterizer{
            .depthClampEnable = vk::False,
            .rasterizerDiscardEnable = vk::False,
            .polygonMode = vk::PolygonMode::eFill,
            .cullMode = vk::CullModeFlagBits::eBack,
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
        vk::PipelineColorBlendAttachmentState colorBlendAttachment{
            .blendEnable = vk::False, //disable is for opaque stuff, enable if there's transparent stuff
            .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        };
        ///if there's multiple image in a draw (like G-buffer), use an array of
        ///PipelineMultisampleStateCreateInfo, the order of it decide which slot is it for
        vk::PipelineColorBlendStateCreateInfo colorBlending{
            .logicOpEnable = vk::False,
            .logicOp = vk::LogicOp::eCopy, //does this even do anything when logicOpEnable is false?
            .attachmentCount = 1, //array.size() when there's multiple
            .pAttachments = &colorBlendAttachment // <- in that case put the array here
        };

        //what is not baked into the pso and instead set during command recording
        //here, the viewport and scissor is dynamic, so resizing the window doesn't require
        //recreating the graphics pipeline
        std::vector dynamicStates = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
        };
        vk::PipelineDynamicStateCreateInfo dynamicState{
            .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
            .pDynamicStates = dynamicStates.data()
        };
        //create descriptor set, change this later
        std::vector<vk::raii::DescriptorSetLayout> descriptor_set_layouts;
        descriptor_set_layouts.emplace_back(createDescriptorSetLayout(device));
        std::array<vk::DescriptorSetLayout, 1> descriptor_set_layouts_raw{
            vk::DescriptorSetLayout{*descriptor_set_layouts.front()}
        };

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo{
            .setLayoutCount = static_cast<uint32_t>(descriptor_set_layouts_raw.size()),
            .pSetLayouts = descriptor_set_layouts_raw.data(),
            .pushConstantRangeCount = 0,
            //.pPushConstantRanges = //fill this when adding push constant
        };

        auto pipelineLayout = vk::raii::PipelineLayout(device, pipelineLayoutInfo);

        vk::PipelineDepthStencilStateCreateInfo depthStencil{
            .depthTestEnable = vk::True,
            .depthWriteEnable = vk::True,
            .depthCompareOp = vk::CompareOp::eLess,
            .depthBoundsTestEnable = vk::False,
            .stencilTestEnable = vk::False
        };

        vk::Format depthFormat = findDepthFormat(physical_device);

        vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipelineCreateInfoChain = {
            {
                .stageCount = 2,
                .pStages = shader_stages,
                .pVertexInputState = &vertexInputInfo,
                .pInputAssemblyState = &inputAssembly,
                .pViewportState = &viewportState,
                .pRasterizationState = &rasterizer,
                .pMultisampleState = &multisampling,
                .pDepthStencilState = &depthStencil,
                .pColorBlendState = &colorBlending,
                .pDynamicState = &dynamicState,
                .layout = pipelineLayout,
                .renderPass = nullptr
            },
            {
                .colorAttachmentCount = 1, //prob need to change this too if there's multiple image
                .pColorAttachmentFormats = &surface_format.format,
                .depthAttachmentFormat = depthFormat
            }
        };

        auto graphicsPipeline = vk::raii::Pipeline(device, nullptr,
                                                   pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>());
        return {
            .descriptor_set_layouts = std::move(descriptor_set_layouts),
            .layout = std::move(pipelineLayout),
            .handle = std::move(graphicsPipeline)
        };
    }
}
