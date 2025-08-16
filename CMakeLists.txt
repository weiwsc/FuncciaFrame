cmake_minimum_required(VERSION 3.31)
project(FuncciaFrame)

set(CMAKE_CXX_STANDARD 23)


find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
pkg_check_modules(SDL3_IMAGE REQUIRED IMPORTED_TARGET sdl3-image)
pkg_check_modules(SDL3_TTF REQUIRED IMPORTED_TARGET sdl3-ttf)
pkg_check_modules(SDL3_MIXER REQUIRED IMPORTED_TARGET sdl3-mixer)

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
        test/Object2.h)

target_link_libraries(FuncciaFrame PRIVATE PkgConfig::SDL3 "-framework OpenGL")
target_link_libraries(FuncciaFrame PRIVATE PkgConfig::SDL3_IMAGE)
target_link_libraries(FuncciaFrame PRIVATE PkgConfig::SDL3_TTF)
target_link_libraries(FuncciaFrame PRIVATE PkgConfig::SDL3_MIXER)