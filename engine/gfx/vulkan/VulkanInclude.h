//
// Created by Wangsicong Wei on 2026-06-11.
//
#pragma once
// Single entry point for Vulkan-Hpp. USE_CPP20_MODULES is defined by the
// vulkan_hpp_module CMake target; without it, or under an IDE parser that
// cannot import modules, fall back to the header.
//
// The named module exports the vk:: API but not the C layer, so the C header
// is included in both branches: it provides the VK_* macros (VK_MAKE_VERSION,
// VKAPI_ATTR) and the global uint32_t/size_t typedefs the code relies on.
#include <cstddef>
#include <cstdint>
#include <vulkan/vulkan.h>

#ifndef VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#endif
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan;
#endif
