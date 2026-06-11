cmake_minimum_required(VERSION 3.31)
project(FuncciaFrame)

set(CMAKE_CXX_STANDARD 23)

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
find_package(harfbuzz CONFIG REQUIRED)
find_package(Freetype CONFIG REQUIRED)
find_package(re2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

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
        graphic/RenderTypes.h
        graphic/RendererInterfaces.h
        graphic/GraphicsDevice.cpp
        graphic/GraphicsDevice.h
        graphic/gl/GLGraphicsDevice.cpp
        graphic/gl/GLGraphicsDevice.h
        App.cpp
        App.h
        util/FpsCounter.cpp
        util/FpsCounter.h)



target_link_libraries(FuncciaFrame PRIVATE
        SDL3::SDL3
        Freetype::Freetype
        harfbuzz::harfbuzz
        re2::re2
        nlohmann_json::nlohmann_json
)
if(APPLE)
    target_link_libraries(FuncciaFrame PRIVATE "-framework OpenGL")
elseif(WIN32)
    find_package(OpenGL REQUIRED)
    target_link_libraries(FuncciaFrame PRIVATE OpenGL::GL)
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(FuncciaFrame PRIVATE OpenGL::GL)
endif()

target_sources(FuncciaFrame PRIVATE external/glad/src/glad.c)
