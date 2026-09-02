cmake_minimum_required(VERSION 4.2)

# import std is experimental; UUID is version-specific and must precede project().
if(CMAKE_VERSION VERSION_GREATER_EQUAL 4.4)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")
elseif(CMAKE_VERSION VERSION_GREATER_EQUAL 4.3)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
else()
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
endif()
set(CMAKE_CXX_MODULE_STD ON)

if(APPLE AND NOT CMAKE_CXX_STDLIB_MODULES_JSON)
    foreach(LLVM_PREFIX "/opt/homebrew/opt/llvm" "/usr/local/opt/llvm")
        if(EXISTS "${LLVM_PREFIX}/lib/c++/libc++.modules.json")
            set(CMAKE_CXX_STDLIB_MODULES_JSON "${LLVM_PREFIX}/lib/c++/libc++.modules.json")
            break()
        endif()
    endforeach()
endif()

project(FuncciaFrame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(warnings)                       # cmake/warnings.cmake

find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)
find_package(Freetype CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(quill CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(Slang REQUIRED)            # cmake/FindSlang.cmake; needs Vulkan first

add_subdirectory(engine)
add_subdirectory(apps)