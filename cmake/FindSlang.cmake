# Locates the Slang compiler shipped with the Vulkan SDK. Provides Slang::Slang.
if(DEFINED ENV{VULKAN_SDK})
    set(_vva_vk_sdk "$ENV{VULKAN_SDK}")
elseif(Vulkan_INCLUDE_DIR)
    cmake_path(GET Vulkan_INCLUDE_DIR PARENT_PATH _vva_vk_sdk)
endif()

find_path(Slang_INCLUDE_DIR NAMES slang.h
        HINTS "${_vva_vk_sdk}/include" "${_vva_vk_sdk}/include/slang")
find_library(Slang_LIBRARY NAMES slang-compiler slang
        HINTS "${_vva_vk_sdk}/lib")
if(WIN32)
    find_file(Slang_DLL NAMES slang-compiler.dll slang.dll
            HINTS "${_vva_vk_sdk}/bin" "${_vva_vk_sdk}/lib")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Slang REQUIRED_VARS Slang_LIBRARY Slang_INCLUDE_DIR)

if(Slang_FOUND AND NOT TARGET Slang::Slang)
    add_library(Slang::Slang SHARED IMPORTED)
    set_target_properties(Slang::Slang PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${Slang_INCLUDE_DIR}")
    if(WIN32)
        set_target_properties(Slang::Slang PROPERTIES
                IMPORTED_IMPLIB "${Slang_LIBRARY}" IMPORTED_LOCATION "${Slang_DLL}")
    else()
        set_target_properties(Slang::Slang PROPERTIES IMPORTED_LOCATION "${Slang_LIBRARY}")
    endif()
endif()
mark_as_advanced(Slang_INCLUDE_DIR Slang_LIBRARY Slang_DLL)