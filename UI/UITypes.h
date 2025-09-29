//
// Created by Wangsicong Wei on 2025-09-26.
//

#ifndef FUNCCIAFRAME_UITYPES_H
#define FUNCCIAFRAME_UITYPES_H


namespace Funccia::UI {
    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    using vec4 = glm::vec4;
    using quat = glm::quat;
    using mat4 = glm::mat4;

    struct Shadow {
        vec4 shadow_color;
        vec2 shadow_offset;
        float blur_radius;
        float spread;
    };
    enum class Display {
        Vertical,
        Horizontal,
        Flex
    };
    enum class Position {
        Absolute,
        Relative
    };

    enum class MarginAuto {
        None,
        All,
        Horizontal,
        Vertical
    };

    enum class Axis {
        Horizontal,
        Vertical
    };

    enum class SizingType {
        Fit,
        Grow,
        Shrink,
        Fixed
    };

    class Sizing {
    public:
        Sizing() = default;
        Sizing(SizingType x_type, SizingType y_type, float x_value, float y_value)
        : x_type(x_type), y_type(y_type), x_value(x_value), y_value(y_value) {}
        ~Sizing() = default;

        auto TypeOnAxis(Axis axis) -> SizingType {
            switch (axis) {
                case Axis::Horizontal:
                    return x_type;
                case Axis::Vertical:
                    return y_type;
                default:
                    return SizingType::Fit;
            }
        }
        auto ValueOnAxis(Axis axis) -> float {
            switch (axis) {
                case Axis::Horizontal:
                    return x_value;
                case Axis::Vertical:
                    return y_value;
                default:
                    return 0;
            }
        }

        SizingType x_type {SizingType::Fit};
        SizingType y_type {SizingType::Fit};
        float x_value;
        float y_value;
    };

    enum class Tag {
        None,
        Label,         // label is horizontally grown, vertically fixed
        Paragraph,     // paragraph is grown on both axes
        AutoMargin
    };


}

#endif //FUNCCIAFRAME_UITYPES_H