# Vulkan-Hpp as a named module. vulkan.cppm declares `export module vulkan;`
# and `export import std;`, so it needs CMAKE_CXX_MODULE_STD (set at the root).
# Consumers reach it through VulkanInclude.h, which does `import vulkan;`
# when USE_CPP20_MODULES is defined and falls back to the header otherwise.
add_library(vulkan_hpp_module STATIC)
target_sources(vulkan_hpp_module PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES
        BASE_DIRS "${Vulkan_INCLUDE_DIR}"
        FILES "${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm")
target_compile_definitions(vulkan_hpp_module
        PUBLIC
            VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
            VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
            USE_CPP20_MODULES
        PRIVATE
            VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING=   # silence the "module is experimental" banner
)
target_link_libraries(vulkan_hpp_module PUBLIC Vulkan::Vulkan)
target_compile_features(vulkan_hpp_module PUBLIC cxx_std_23)

add_library(vva_gfx_vulkan STATIC
        VulkanRenderer.cpp
        VulkanInstance.cpp
        VulkanDevice.cpp
        VulkanSwapChain.cpp
        VulkanImage.cpp
        VulkanFrame.cpp
        VulkanUploadContext.cpp
        VulkanAllocator.cpp
        VmaImplementation.cpp
        AllocatedBuffer.cpp
        AllocatedImage.cpp
        SlangShaderCompiler.cpp
        GraphicsPipeline.cpp
        DescriptorAllocator.cpp
        types/Param.cpp
        types/Model.cpp
        types/Texture.cpp
        types/Shader.cpp
        util/VulkanLayerUtil.cpp
        util/VulkanInstanceExtensionUtil.cpp
        util/VulkanPhysicalDeviceUtil.cpp
        util/VulkanLogicalDeviceUtil.cpp
        util/VulkanDebugMessengerUtil.cpp
)
add_library(vva::gfx::vulkan ALIAS vva_gfx_vulkan)

target_link_libraries(vva_gfx_vulkan
        PUBLIC
            vva::gfx
            vva::platform                       # WindowInterface in VulkanRenderer.h
            vulkan_hpp_module                   # vk:: types in every public header
            GPUOpen::VulkanMemoryAllocator      # vk_mem_alloc.h in AllocatedBuffer.h / AllocatedImage.h
            Slang::Slang                        # slang.h in SlangShaderCompiler.h
        PRIVATE
            tinyobjloader::tinyobjloader
            $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
)
set_target_properties(vva_gfx_vulkan PROPERTIES CXX_SCAN_FOR_MODULES ON)
vva_configure_target(vva_gfx_vulkan)
