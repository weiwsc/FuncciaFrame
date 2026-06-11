//
// Created by Wangsicong Wei on 2025-09-26.
//

#ifndef FUNCCIAFRAME_SIDEOFFSET_H
#define FUNCCIAFRAME_SIDEOFFSET_H
#include "../graphic/MathTypes.h"
#include "UITypes.h"

namespace Funccia::UI {
    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    using vec4 = glm::vec4;
    using quat = glm::quat;
    using mat4 = glm::mat4;

    class SideOffset {
    public:
        using value_type = float;

        constexpr SideOffset() noexcept : v(0,0,0,0) {}
        constexpr explicit SideOffset(value_type all) noexcept : v(all,all,all,all) {}
        constexpr SideOffset(value_type topBottom, value_type rightLeft) noexcept
            : v(topBottom, rightLeft, topBottom, rightLeft) {}
        constexpr SideOffset(value_type top, value_type right, value_type bottom, value_type left) noexcept
            : v(top, right, bottom, left) {}

        [[nodiscard]] auto top() const -> value_type {return v.x;}
        [[nodiscard]] auto right() const -> value_type {return v.y;}
        [[nodiscard]] auto bottom() const -> value_type {return v.z;}
        [[nodiscard]] auto left() const -> value_type {return v.w;}

        [[nodiscard]] auto first(Axis onAxis) const -> value_type;
        [[nodiscard]] auto second(Axis onAxis) const -> value_type;
        [[nodiscard]] auto firstAndSecond(Axis onAxis) const -> value_type;
        ///
        /// @return total horizontal offset
        [[nodiscard]] constexpr value_type horizontal() const noexcept { return v.y + v.w; }
        ///
        /// @return total vertical offset
        [[nodiscard]] constexpr value_type vertical()   const noexcept { return v.x + v.z; }

        auto SetTop(value_type _top) -> void {v.x = _top;}
        auto SetRight(value_type _right) -> void {v.y = _right;}
        auto SetBottom(value_type _bottom) -> void {v.z = _bottom;}
        auto SetLeft(value_type _left) -> void {v.w = _left;}

        auto SetAll(value_type _all) ->void {
            v.x = _all;
            v.y = _all;
            v.z = _all;
            v.w = _all;
        }
        auto Set(value_type _top, value_type _right, value_type _bottom, value_type _left) -> void {
            v.x = _top;
            v.y = _right;
            v.z = _bottom;
            v.w = _left;
        }
        auto SetHorizontal(value_type _horizontal) -> void {
            v.y = _horizontal;
            v.w = _horizontal;
        }
        auto SetVertical(value_type _vertical) -> void {
            v.x = _vertical;
            v.z = _vertical;
        }
    private:
        vec4 v {0,0,0,0};
    };

    class PositionOffset {
    public:
        PositionOffset() = default;
        PositionOffset(float x, float y) : v(x,y) {}
        [[nodiscard]] auto OnAxis(Axis onAxis) const -> float {
            switch (onAxis) {
                case Axis::Horizontal:
                    return v.x;
                case Axis::Vertical:
                    return v.y;
                default:
                    return 0;
            }
        }
        auto Set(Axis onAxis, float value) -> void {
            switch (onAxis) {
                case Axis::Horizontal:
                    v.x = value;
                    break;
                case Axis::Vertical:
                    v.y = value;
                    break;
            }
        }
        [[nodiscard]] auto GetX() const -> float {return v.x;}
        [[nodiscard]] auto GetY() const -> float {return v.y;}
    private:
        vec2 v {0,0};
    };
}
#endif //FUNCCIAFRAME_SIDEOFFSET_H