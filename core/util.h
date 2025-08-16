//
// Created by Wangsicong Wei on 2025-06-18.
//

#ifndef UTIL_H
#define UTIL_H

#if defined(_WIN32) || defined(_WIN64)
#define FF_PLATFROM_WINDOWS

#elif defined(__APPLE__) || defined(__linux__)
#define FF_PLATFORM_UNIX

#endif

#ifdef FF_PLATFROM_WINDOWS
#include <Windows.h>
#include <direct.h>
#define FF_ASSERT(_cond, _msg) \
if (!(_cond)) {OutputDebugStringA(_msg); std::abort();}
#define GetCurrentDir _getcwd
#endif

#ifdef FF_PLATFORM_UNIX
#include <string>


// #define FF_ASSERT(_cond, _msg) \
// if (!(_cond)) {std::cerr << _msg << std::endl;; std::abort();}

void FF_ASSERT(bool _cond, const std::string& _msg);
auto toReadableSize(std::size_t _size) -> std::string;
#define GetCurrentDir getcwd
#endif



#endif //UTIL_H
