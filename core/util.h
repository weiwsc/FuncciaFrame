//
// Created by Wangsicong Wei on 2025-06-18.
//
#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <cstdlib>
#include <cstddef>

#if defined(_WIN32) || defined(_WIN64)
#define FF_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__linux__)
#define FF_PLATFORM_UNIX
#endif

#ifdef FF_PLATFORM_WINDOWS
#define NOMINMAX
#include <Windows.h>
#include <direct.h>
#define FF_ASSERT(_cond, _msg) \
    if (!(_cond)) { OutputDebugStringA(_msg); std::abort(); }
#define GetCurrentDir _getcwd
#endif

#ifdef FF_PLATFORM_UNIX
void FF_ASSERT(bool _cond, const std::string& _msg);
auto toReadableSize(std::size_t _size) -> std::string;
#define GetCurrentDir getcwd
#endif

class Utilities {
public:
	static auto toReadableSize(std::size_t _size) -> std::string;
};

#endif // UTIL_H