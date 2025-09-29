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

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
pkg_check_modules(SDL3_IMAGE REQUIRED IMPORTED_TARGET sdl3-image)
pkg_check_modules(SDL3_TTF REQUIRED IMPORTED_TARGET sdl3-ttf)
pkg_check_modules(SDL3_MIXER REQUIRED IMPORTED_TARGET sdl3-mixer)
pkg_check_modules(FT2 REQUIRED freetype2)
pkg_check_modules(HARFBUZZ REQUIRED harfbuzz)
#pkg_check_modules(GLEW REQUIRED IMPORTED_TARGET glew)
include_directories(external/glad/include)
include_directories(external/std_image/include)

find_package(harfbuzz REQUIRED)
find_package(glfw3 3.4 REQUIRED)

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
        test/ContainerA.cpp
        test/ContainerA.h
        test/Object1.cpp
        test/Object1.h
        test/Object2.h
        test/OpenGLtest.cpp
        test/OpenGLtest.h
        test/TestRun.cpp
        test/TestRun.h
        graphic/gl/WindowController.cpp
        graphic/gl/WindowController.h
        graphic/gl/mesh.cpp
        graphic/gl/mesh.h
        graphic/gl/ShaderUtil.cpp
        graphic/gl/ShaderUtil.h
        graphic/gl/Shader.cpp
        graphic/gl/Shader.h
        core/TextFile.cpp
        core/TextFile.h
        external/std_image/include/std_image.cpp
        testGround.cpp
        graphic/gl/Transform.cpp
        graphic/gl/Transform.h
        graphic/gl/MathTypes.cpp
        graphic/gl/MathTypes.h
        UI/UIElement.cpp
        UI/UIElement.h
        graphic/gl/FontAtlas.cpp
        graphic/gl/FontAtlas.h
        graphic/gl/TextRenderer.cpp
        graphic/gl/TextRenderer.h
        external/std_image/include/stb_image_write.cpp
        graphic/text/GlyphAtlas.cpp
        graphic/text/GlyphAtlas.h
        UI/SideOffset.cpp
        UI/SideOffset.h
        UI/UITypes.h
        graphic/gl/UIRender.cpp
        graphic/gl/UIRender.h
        UI/Window.cpp
        UI/Window.h
        UI/UIElementFluentAPI.cpp
        UI/UIElementTemplates.cpp
        graphic/text/HBShaper.cpp
        graphic/text/HBShaper.h
        graphic/text/Glyph.cpp
        graphic/text/Glyph.h)

target_link_libraries(FuncciaFrame PRIVATE PkgConfig::SDL3 "-framework OpenGL")
target_link_libraries(FuncciaFrame PRIVATE PkgConfig::SDL3_IMAGE)
target_link_libraries(FuncciaFrame PRIVATE PkgConfig::SDL3_TTF)
target_link_libraries(FuncciaFrame PRIVATE PkgConfig::SDL3_MIXER)
target_link_libraries(FuncciaFrame PRIVATE glfw)
target_link_libraries(FuncciaFrame
        PRIVATE
        harfbuzz
)

#target_link_libraries(FuncciaFrame PRIVATE PkgConfig::GLEW)
target_include_directories(FuncciaFrame PRIVATE ${FT2_INCLUDE_DIRS})
target_link_directories(FuncciaFrame PRIVATE ${FT2_LIBRARY_DIRS})
target_link_libraries(FuncciaFrame PRIVATE ${FT2_LIBRARIES})

target_include_directories(FuncciaFrame PRIVATE ${HARFBUZZ_INCLUDE_DIRS})
target_link_directories(FuncciaFrame PRIVATE ${HARFBUZZ_LIBRARY_DIRS})
target_link_libraries(FuncciaFrame PRIVATE ${HARFBUZZ_LIBRARIES})

target_sources(FuncciaFrame PRIVATE external/glad/src/glad.c)