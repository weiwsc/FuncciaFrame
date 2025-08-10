//
// Created by Wangsicong Wei on 2025-06-18.
//

#include "util.h"
#include <iostream>
#include <__ostream/basic_ostream.h>

void FF_ASSERT(bool _cond, const std::string &_msg) {
    if (!(_cond)) {std::cerr << _msg << std::endl;; std::abort();}
}
