//
// Created by Wangsicong Wei on 2025-09-26.
//

#include "SideOffset.h"

namespace Funccia::UI {
    auto SideOffset::first(Axis onAxis) const -> value_type {
        switch (onAxis) {
            case Axis::Horizontal:
                return left();
            case Axis::Vertical:
                return top();
            default:
                return 0;
        }
    }

    auto SideOffset::second(Axis onAxis) const -> value_type {
        switch (onAxis) {
            case Axis::Horizontal:
                return right();
            case Axis::Vertical:
                return bottom();
            default:
                return 0;
        }
    }

    auto SideOffset::firstAndSecond(Axis onAxis) const -> value_type {
        return first(onAxis) + second(onAxis);
    }
}
