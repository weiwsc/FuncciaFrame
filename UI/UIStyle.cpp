//
// Created by Wangsicong Wei on 2026-01-09.
//

#include "UIStyle.h"
namespace Funccia::UI {
    auto UIStyle::SetTag(UI::Tag _tag) -> UIStyle & {
        tag = _tag;
        return *this;
    }

    auto UIStyle::Text(const std::string &_text) -> UIStyle & {
        text = _text;
        return *this;
    }

    auto UIStyle::FontSize(int size) -> UIStyle & {
        font_size = size;
        return *this;
    }

    auto UIStyle::TextWrap(Graphic::GL::TextWrap wrap) -> UIStyle & {
        text_wrap_mode = wrap;
        return *this;
    }

    auto UIStyle::InvisibleButOccupySpace(bool option) -> UIStyle & {
        invisible_but_occupy_space = option;
        return *this;
    }

    auto UIStyle::MarginTop(float value) -> UIStyle & {
        margin.SetTop(value);
        return *this;
    }

    auto UIStyle::MarginLeft(float value) -> UIStyle & {
        margin.SetLeft(value);
        return *this;
    }

    auto UIStyle::MarginRight(float value) -> UIStyle & {
        margin.SetRight(value);
        return *this;
    }

    auto UIStyle::MarginBottom(float value) -> UIStyle & {
        margin.SetBottom(value);
        return *this;
    }

    auto UIStyle::Margin(float all_values) -> UIStyle & {
        margin = {all_values, all_values, all_values, all_values};
        return *this;
    }

    auto UIStyle::Margin(float topButton, float leftRight) -> UIStyle & {
        margin = {topButton, leftRight, topButton, leftRight};
        return *this;
    }

    auto UIStyle::Margin(float top, float right, float bottom, float left) -> UIStyle & {
        margin = {top, right, bottom, left};
        return *this;
    }

    auto UIStyle::PaddingTop(float value) -> UIStyle & {
        padding.SetTop(value);
        return *this;
    }

    auto UIStyle::PaddingLeft(float value) -> UIStyle & {
        padding.SetLeft(value);
        return *this;
    }

    auto UIStyle::PaddingRight(float value) -> UIStyle & {
        padding.SetRight(value);
        return *this;
    }

    auto UIStyle::PaddingBottom(float value) -> UIStyle & {
        padding.SetBottom(value);
        return *this;
    }


    auto UIStyle::Padding(float all_values) -> UIStyle & {
        padding = {all_values, all_values, all_values, all_values};
        return *this;
    }

    auto UIStyle::Padding(float topButton, float leftRight) -> UIStyle & {
        padding = {topButton, leftRight, topButton, leftRight};
        return *this;
    }

    auto UIStyle::Padding(float top, float right, float bottom, float left) -> UIStyle & {\
        padding = {top, right, bottom, left};
        return *this;
    }

    auto UIStyle::Background(vec4 _color) -> UIStyle & {
        background = _color;
        return *this;
    }

    auto UIStyle::TextColor(vec4 _color) -> UIStyle & {
        color = _color;
        return *this;
    }

    auto UIStyle::BoxShadow(vec2 offSet, float blur, float spread, vec4 _color) -> UIStyle & {
        shadow.shadow_offset = offSet;
        shadow.blur_radius = blur;
        shadow.spread = spread;
        shadow.shadow_color = _color;
        return *this;
    }

    auto UIStyle::BoxShadowColor(vec4 _color) -> UIStyle & {
        shadow.shadow_color = _color;
        return *this;
    }

    auto UIStyle::BoxShadowBlur(float blur) -> UIStyle & {
        shadow.blur_radius = blur;
        return *this;
    }

    auto UIStyle::BoxShadowSpread(float spread) -> UIStyle & {
        shadow.spread = spread;
        return *this;
    }

    auto UIStyle::BoxShadowOffset(vec2 offSet) -> UIStyle & {
        shadow.shadow_offset = offSet;
        return *this;
    }

    auto UIStyle::BorderRadius(vec4 radius) -> UIStyle & {
        border_radius = radius;
        return *this;
    }

    auto UIStyle::BorderColor(vec4 _color) -> UIStyle & {
        border_color = _color;
        return *this;
    }

    auto UIStyle::BorderWidth(vec4 width) -> UIStyle & {
        border_widths.SetTop(width.x);
        border_widths.SetRight(width.y);
        border_widths.SetBottom(width.z);
        border_widths.SetLeft(width.w);
        return *this;
    }

    auto UIStyle::HorizontalFixed(float size) -> UIStyle & {
        sizing.x_type = SizingType::Fixed;
        sizing.x_value = size;
        return *this;
    }

    auto UIStyle::HorizontalGrow() -> UIStyle & {
        sizing.x_type = SizingType::Grow;
        return *this;
    }

    auto UIStyle::HorizontalFit() -> UIStyle & {
        sizing.x_type = SizingType::Fit;
        return *this;
    }

    auto UIStyle::VerticalFixed(float size) -> UIStyle & {
        sizing.y_type = SizingType::Fixed;
        sizing.y_value = size;
        return *this;
    }

    auto UIStyle::VerticalGrow() -> UIStyle & {
        sizing.y_type = SizingType::Grow;
        return *this;
    }

    auto UIStyle::VerticalFit() -> UIStyle & {
        sizing.y_type = SizingType::Fit;
        return *this;
    }

    auto UIStyle::HorizontalStack() -> UIStyle & {
        display_axis = Axis::Horizontal;
        return *this;
    }

    auto UIStyle::VerticalStack() -> UIStyle & {
        display_axis = Axis::Vertical;
        return *this;
    }
}