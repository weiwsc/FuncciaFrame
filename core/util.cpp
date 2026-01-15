//
// Created by Wangsicong Wei on 2025-06-18.
//


#include "util.h"
#include <iostream>

#ifndef FF_PLATFORM_WINDOWS

void FF_ASSERT(bool _cond, const std::string &_msg) {
    if (!(_cond)) {std::cerr << _msg << std::endl;; std::abort();}
}
#endif // !FF_PLATFORM_WINDOWS
auto Utilities::toReadableSize(std::size_t _size) -> std::string {
    if (_size < 1024) {
        return std::to_string(_size) + "B";
    }
    else if (_size < 1024 * 1024) {
        return std::to_string(_size / 1024) + "KB";
    }
    else if (_size < 1024 * 1024 * 1024) {
        return std::to_string(_size / (1024 * 1024)) + "MB";
    }
    else {
        return std::to_string(_size / (1024 * 1024 * 1024)) + "GB";
    }
}