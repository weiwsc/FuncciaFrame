cmake_minimum_required (VERSION 4.2.3)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

if(CMAKE_VERSION VERSION_GREATER_EQUAL 4.4)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")
elseif(CMAKE_VERSION VERSION_GREATER_EQUAL 4.3)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
else()
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
endif()
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_MODULE_STD ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

if(APPLE AND NOT CMAKE_CXX_STDLIB_MODULES_JSON)
    foreach(LLVM_PREFIX "/opt/homebrew/opt/llvm" "/usr/local/opt/llvm")
        if(EXISTS "${LLVM_PREFIX}/lib/c++/libc++.modules.json")
            set(CMAKE_CXX_STDLIB_MODULES_JSON "${LLVM_PREFIX}/lib/c++/libc++.modules.json")
            break()
        endif()
    endforeach()
endif()

project(FuncciaFrame)


if(APPLE)
    execute_process(
            COMMAND xcrun --sdk macosx --show-sdk-path
            OUTPUT_VARIABLE MACOSX_SDK_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(MACOSX_SDK_PATH)
        set(CMAKE_OSX_SYSROOT "${MACOSX_SDK_PATH}" CACHE PATH "macOS SDK" FORCE)
    endif()
endif()


find_package(SDL3 CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)
find_package(Freetype CONFIG REQUIRED)
find_package(re2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(quill CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)

if(DEFINED ENV{VULKAN_SDK})
    set(_vk_sdk "$ENV{VULKAN_SDK}")
else()
    cmake_path(GET Vulkan_INCLUDE_DIR PARENT_PATH _vk_sdk)
endif()

find_path(SLANG_INCLUDE_DIR
        NAMES slang.h
        HINTS "${_vk_sdk}/include" "${_vk_sdk}/include/slang"
        REQUIRED)

find_library(SLANG_LIBRARY
        NAMES slang-compiler slang
        HINTS "${_vk_sdk}/lib"
        REQUIRED)

add_library(VulkanCppModule)
add_library(Vulkan::cppm ALIAS VulkanCppModule)
add_library(slang::slang SHARED IMPORTED)

set_target_properties(slang::slang PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SLANG_INCLUDE_DIR}")
if(WIN32)
    # find_library returns the import lib on Windows; the DLL lives in bin/
    find_file(SLANG_DLL NAMES slang-compiler.dll slang.dll
            HINTS "${_vk_sdk}/bin" "${_vk_sdk}/lib" REQUIRED)
    set_target_properties(slang::slang PROPERTIES
            IMPORTED_IMPLIB   "${SLANG_LIBRARY}"
            IMPORTED_LOCATION "${SLANG_DLL}")
else()
    set_target_properties(slang::slang PROPERTIES
            IMPORTED_LOCATION "${SLANG_LIBRARY}")
endif()

target_compile_definitions(VulkanCppModule PUBLIC
        VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
        VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
)



target_include_directories(VulkanCppModule PRIVATE "${Vulkan_INCLUDE_DIR}")
target_link_libraries(VulkanCppModule PUBLIC Vulkan::Vulkan)

set_target_properties(VulkanCppModule PROPERTIES CXX_STANDARD 23)
target_sources(VulkanCppModule
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES
        BASE_DIRS "${Vulkan_INCLUDE_DIR}"
        FILES "${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm"
)

include_directories(external/glad/include)
include_directories(external/std_image/include)

add_executable(FuncciaFrame main.cpp
        core/FileController.cpp
        core/FileController.h
        core/Singleton.h
        core/util.cpp
        core/util.h
        core/AssetController.cpp
        core/AssetController.h
        core/StackAllocator.cpp
        core/StackAllocator.h
        core/Asset.cpp
        core/Asset.h
        core/ObjectPool.h
        core/ISerializable.h
        core/Resource.cpp
        core/Resource.h
        core/Serializer.cpp
        core/Serializer.h
        core/ObjectPoolManager.cpp
        core/ObjectPoolManager.h
        core/TypeRegistry.cpp
        core/TypeRegistry.h
        graphic/WindowController.cpp
        graphic/WindowController.h
        graphic/gl/Mesh.cpp
        graphic/gl/Mesh.h
        graphic/gl/ShaderUtil.cpp
        graphic/gl/ShaderUtil.h
        graphic/gl/Shader.cpp
        graphic/gl/Shader.h
        core/TextFile.cpp
        core/TextFile.h
        external/std_image/include/std_image.cpp
        graphic/Transform.cpp
        graphic/Transform.h
        graphic/MathTypes.cpp
        graphic/MathTypes.h
        UI/UIElement.cpp
        UI/UIElement.h
        graphic/gl/GLTextRenderer.cpp
        graphic/gl/GLTextRenderer.h
        external/std_image/include/stb_image_write.cpp
        graphic/gl/GLGlyphAtlas.cpp
        graphic/gl/GLGlyphAtlas.h
        UI/SideOffset.cpp
        UI/SideOffset.h
        UI/UITypes.h
        graphic/gl/GLUiRenderer.cpp
        graphic/gl/GLUiRenderer.h
        UI/Window.cpp
        UI/Window.h
        UI/UIElementFluentAPI.cpp
        UI/UIElementTemplates.cpp
        graphic/text/HBShaper.cpp
        graphic/text/HBShaper.h
        graphic/text/Glyph.cpp
        graphic/text/Glyph.h
        UI/UIHelper.cpp
        UI/UIHelper.h
        graphic/WindowInterface.h
        graphic/gl/GLFWWindow.cpp
        graphic/gl/GLFWWindow.h
        graphic/gl/SDLWindow.cpp
        graphic/gl/SDLWindow.h
        graphic/text/IRectPacker.h
        graphic/text/GridRectPacker.cpp
        graphic/text/GridRectPacker.h
        input/MouseState.cpp
        input/MouseState.h
        UI/JsonUiBuilder.cpp
        UI/JsonUiBuilder.h
        UI/UIStyle.cpp
        UI/UIStyle.h
        UI/UIFluentAPI.cpp
        UI/UIFluentAPI.h
        UI/UIStyleDef.cpp
        UI/UIStyleDef.h
        core/FileWatcher.cpp
        core/FileWatcher.h
        UI/UIState.cpp
        UI/UIState.h
        UI/UIStyleSheets.cpp
        UI/UIStyleSheets.h
        graphic/text/FontManager.cpp
        graphic/text/FontManager.h
        core/ProfileTimer.cpp
        core/ProfileTimer.h
        input/Input.cpp
        input/Input.h
        graphic/RenderBackend.h
        graphic/RenderTypes.h
        graphic/RendererInterfaces.h
        graphic/GraphicsDevice.cpp
        graphic/GraphicsDevice.h
        graphic/gl/GLGraphicsDevice.cpp
        graphic/gl/GLGraphicsDevice.h
        App.cpp
        App.h
        util/FpsCounter.cpp
        util/FpsCounter.h
        graphic/vulkan/VulkanRenderer.cpp
        graphic/vulkan/VulkanRenderer.h
        graphic/vulkan/VulkanInstance.cpp
        graphic/vulkan/VulkanInstance.h
        graphic/vulkan/VulkanInclude.h
        ApplicationMetadata.h
        graphic/vulkan/util/VulkanLayerUtil.cpp
        graphic/vulkan/util/VulkanLayerUtil.h
        graphic/vulkan/util/VulkanInstanceExtensionUtil.cpp
        graphic/vulkan/util/VulkanInstanceExtensionUtil.h
        graphic/vulkan/util/VulkanPhysicalDeviceUtil.h
        graphic/vulkan/util/VulkanPhysicalDeviceUtil.cpp
        graphic/vulkan/util/VulkanDebugMessengerUtil.cpp
        graphic/vulkan/util/VulkanDebugMessengerUtil.h
        graphic/vulkan/VulkanDevice.cpp
        graphic/vulkan/VulkanDevice.h
        graphic/vulkan/util/VulkanLogicalDeviceUtil.cpp
        graphic/vulkan/util/VulkanLogicalDeviceUtil.h
        graphic/vulkan/types/VulkanTypesDef.h
        util/Log.cpp
        util/Log.h
        graphic/vulkan/VulkanConfig.h
        graphic/vulkan/VulkanSwapChain.cpp
        graphic/vulkan/VulkanSwapChain.h
        graphic/vulkan/VulkanImage.cpp
        graphic/vulkan/VulkanImage.h
        graphic/vulkan/VulkanFrame.cpp
        graphic/vulkan/VulkanFrame.h
        graphic/vulkan/VulkanUploadContext.cpp
        graphic/vulkan/VulkanUploadContext.h
        graphic/vulkan/VulkanAllocator.cpp
        graphic/vulkan/VulkanAllocator.h
        graphic/vulkan/VmaImplementation.cpp
        graphic/vulkan/AllocatedBuffer.cpp
        graphic/vulkan/AllocatedBuffer.h
        graphic/vulkan/AllocatedImage.cpp
        graphic/vulkan/AllocatedImage.h
        graphic/shaders/SlangShaderCompiler.cpp
        graphic/shaders/SlangShaderCompiler.h
        graphic/vulkan/GraphicsPipeline.cpp
        graphic/vulkan/GraphicsPipeline.h
        graphic/vulkan/types/Param.cpp
        graphic/vulkan/types/Param.h)



target_link_libraries(FuncciaFrame PRIVATE
        Vulkan::Vulkan
        SDL3::SDL3
        Freetype::Freetype
        harfbuzz::harfbuzz
        re2::re2
        nlohmann_json::nlohmann_json
        quill::quill
        GPUOpen::VulkanMemoryAllocator
        slang::slang
)
target_include_directories(FuncciaFrame SYSTEM BEFORE PRIVATE ${CMAKE_SOURCE_DIR}/external/doc_header)
if(APPLE)
    target_link_libraries(FuncciaFrame PRIVATE "-framework OpenGL")
elseif(WIN32)
    find_package(OpenGL REQUIRED)
    target_link_libraries(FuncciaFrame PRIVATE OpenGL::GL)
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(FuncciaFrame PRIVATE OpenGL::GL)
endif()
target_compile_definitions(FuncciaFrame PRIVATE QUILL_DISABLE_NON_PREFIXED_MACROS)
target_sources(FuncciaFrame PRIVATE external/glad/src/glad.c)
