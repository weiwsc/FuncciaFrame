//
// Created by Wangsicong Wei on 2026-01-09.
//

#include "UIStyleDef.h"
#include "UIStyle.h"

namespace Funccia::UI {

    void UIStyleDef::merge(const UIStyleDef& other) {
        if (other.tag) tag = other.tag;
        if (other.margin) margin = other.margin;
        if (other.padding) padding = other.padding;
        if (other.border_widths) border_widths = other.border_widths;
        if (other.display_axis) display_axis = other.display_axis;
        if (other.position) position = other.position;
        if (other.sizing) sizing = other.sizing;
        if (other.overflow) overflow = other.overflow;
        if (other.is_rendered) is_rendered = other.is_rendered;
        if (other.invisible_but_occupy_space) invisible_but_occupy_space = other.invisible_but_occupy_space;
        if (other.background) background = other.background;
        if (other.color) color = other.color;
        if (other.border_color) border_color = other.border_color;
        if (other.border_radius) border_radius = other.border_radius;
        if (other.shadow) shadow = other.shadow;
        //if (other.text) text = other.text;
        if (other.font_size) font_size = other.font_size;
        if (other.text_wrap_mode) text_wrap_mode = other.text_wrap_mode;
        if (other.font_path) font_path = other.font_path;

        if (other.handle_scroll) handle_scroll = other.handle_scroll;
    }

    void UIStyleDef::applyTo(UIStyle& style) const {
        if (tag) style.tag = *tag;
        if (margin) style.margin = *margin;
        if (padding) style.padding = *padding;
        if (border_widths) style.border_widths = *border_widths;
        if (display_axis) style.display_axis = *display_axis;
        if (position) style.position = *position;
        if (sizing) style.sizing = *sizing;
        if (overflow) style.overflow = *overflow;
        if (is_rendered) style.is_rendered = *is_rendered;
        if (invisible_but_occupy_space) style.invisible_but_occupy_space = *invisible_but_occupy_space;
        if (background) style.background = *background;
        if (color) style.color = *color;
        if (border_color) style.border_color = *border_color;
        if (border_radius) style.border_radius = *border_radius;
        if (shadow) style.shadow = *shadow;
        //if (text) style.text = *text;
        if (font_size) style.font_size = *font_size;
        if (text_wrap_mode) style.text_wrap_mode = *text_wrap_mode;
        if (font_path) style.font_path = *font_path;

        if (handle_scroll) style.handle_scroll = *handle_scroll;
    }

    // --- Margin ---

    auto UIStyleDef::MarginTop(float value) -> UIStyleDef& {
        if (!margin) margin = SideOffset{0,0,0,0};
        margin->SetTop(value);
        return *this;
    }

    auto UIStyleDef::MarginLeft(float value) -> UIStyleDef& {
        if (!margin) margin = SideOffset{0,0,0,0};
        margin->SetLeft(value);
        return *this;
    }

    auto UIStyleDef::MarginRight(float value) -> UIStyleDef& {
        if (!margin) margin = SideOffset{0,0,0,0};
        margin->SetRight(value);
        return *this;
    }

    auto UIStyleDef::MarginBottom(float value) -> UIStyleDef& {
        if (!margin) margin = SideOffset{0,0,0,0};
        margin->SetBottom(value);
        return *this;
    }

    auto UIStyleDef::Margin(float all_values) -> UIStyleDef& {
        margin = SideOffset{all_values, all_values, all_values, all_values};
        return *this;
    }

    auto UIStyleDef::Margin(float topBottom, float leftRight) -> UIStyleDef& {
        margin = SideOffset{topBottom, leftRight, topBottom, leftRight};
        return *this;
    }

    auto UIStyleDef::Margin(float top, float right, float bottom, float left) -> UIStyleDef& {
        margin = SideOffset{top, right, bottom, left};
        return *this;
    }

    // --- Padding ---

    auto UIStyleDef::PaddingTop(float value) -> UIStyleDef& {
        if (!padding) padding = SideOffset{0,0,0,0};
        padding->SetTop(value);
        return *this;
    }

    auto UIStyleDef::PaddingLeft(float value) -> UIStyleDef& {
        if (!padding) padding = SideOffset{0,0,0,0};
        padding->SetLeft(value);
        return *this;
    }

    auto UIStyleDef::PaddingRight(float value) -> UIStyleDef& {
        if (!padding) padding = SideOffset{0,0,0,0};
        padding->SetRight(value);
        return *this;
    }

    auto UIStyleDef::PaddingBottom(float value) -> UIStyleDef& {
        if (!padding) padding = SideOffset{0,0,0,0};
        padding->SetBottom(value);
        return *this;
    }

    auto UIStyleDef::Padding(float all_values) -> UIStyleDef& {
        padding = SideOffset{all_values, all_values, all_values, all_values};
        return *this;
    }

    auto UIStyleDef::Padding(float topBottom, float leftRight) -> UIStyleDef& {
        padding = SideOffset{topBottom, leftRight, topBottom, leftRight};
        return *this;
    }

    auto UIStyleDef::Padding(float top, float right, float bottom, float left) -> UIStyleDef& {
        padding = SideOffset{top, right, bottom, left};
        return *this;
    }

    // --- Colors ---

    auto UIStyleDef::Background(vec4 _color) -> UIStyleDef& {
        background = _color;
        return *this;
    }

    auto UIStyleDef::TextColor(vec4 _color) -> UIStyleDef& {
        color = _color;
        return *this;
    }

    // --- Shadow ---

    auto UIStyleDef::BoxShadow(vec2 offSet, float blur, float spread, vec4 _color) -> UIStyleDef& {
        shadow = Shadow{_color, offSet, blur, spread};
        return *this;
    }

    auto UIStyleDef::BoxShadowColor(vec4 _color) -> UIStyleDef& {
        if (!shadow) shadow = Shadow{vec4(0,0,0,0), vec2(0,0), 0, 0};
        shadow->shadow_color = _color;
        return *this;
    }

    auto UIStyleDef::BoxShadowBlur(float blur) -> UIStyleDef& {
        if (!shadow) shadow = Shadow{vec4(0,0,0,0), vec2(0,0), 0, 0};
        shadow->blur_radius = blur;
        return *this;
    }

    auto UIStyleDef::BoxShadowSpread(float spread) -> UIStyleDef& {
        if (!shadow) shadow = Shadow{vec4(0,0,0,0), vec2(0,0), 0, 0};
        shadow->spread = spread;
        return *this;
    }

    auto UIStyleDef::BoxShadowOffset(vec2 offSet) -> UIStyleDef& {
        if (!shadow) shadow = Shadow{vec4(0,0,0,0), vec2(0,0), 0, 0};
        shadow->shadow_offset = offSet;
        return *this;
    }

    // --- Border ---

    auto UIStyleDef::BorderRadius(vec4 radius) -> UIStyleDef& {
        border_radius = radius;
        return *this;
    }

    auto UIStyleDef::BorderColor(vec4 _color) -> UIStyleDef& {
        border_color = _color;
        return *this;
    }

    auto UIStyleDef::BorderWidth(vec4 width) -> UIStyleDef& {
        border_widths = SideOffset{width.x, width.y, width.z, width.w};
        return *this;
    }

    // --- Sizing ---

    auto UIStyleDef::HorizontalFixed(float size) -> UIStyleDef& {
        if (!sizing) sizing = Sizing{SizingType::Grow, SizingType::Fit, 0, 0};
        sizing->x_type = SizingType::Fixed;
        sizing->x_value = size;
        return *this;
    }

    auto UIStyleDef::HorizontalGrow() -> UIStyleDef& {
        if (!sizing) sizing = Sizing{SizingType::Grow, SizingType::Fit, 0, 0};
        sizing->x_type = SizingType::Grow;
        return *this;
    }

    auto UIStyleDef::HorizontalFit() -> UIStyleDef& {
        if (!sizing) sizing = Sizing{SizingType::Grow, SizingType::Fit, 0, 0};
        sizing->x_type = SizingType::Fit;
        return *this;
    }

    auto UIStyleDef::VerticalFixed(float size) -> UIStyleDef& {
        if (!sizing) sizing = Sizing{SizingType::Grow, SizingType::Fit, 0, 0};
        sizing->y_type = SizingType::Fixed;
        sizing->y_value = size;
        return *this;
    }

    auto UIStyleDef::VerticalGrow() -> UIStyleDef& {
        if (!sizing) sizing = Sizing{SizingType::Grow, SizingType::Fit, 0, 0};
        sizing->y_type = SizingType::Grow;
        return *this;
    }

    auto UIStyleDef::VerticalFit() -> UIStyleDef& {
        if (!sizing) sizing = Sizing{SizingType::Grow, SizingType::Fit, 0, 0};
        sizing->y_type = SizingType::Fit;
        return *this;
    }

    // --- Layout direction ---

    auto UIStyleDef::HorizontalStack() -> UIStyleDef& {
        display_axis = Axis::Horizontal;
        return *this;
    }

    auto UIStyleDef::VerticalStack() -> UIStyleDef& {
        display_axis = Axis::Vertical;
        return *this;
    }

    // --- Tag ---

    auto UIStyleDef::SetTag(const std::string& _tag) -> UIStyleDef& {
        tag = _tag;
        return *this;
    }

    // --- Text ---

    //auto UIStyleDef::Text(const std::string& _text) -> UIStyleDef& {
    //    text = _text;
    //    return *this;
    //}

    auto UIStyleDef::FontSize(int size) -> UIStyleDef& {
        font_size = size;
        return *this;
    }

    auto UIStyleDef::TextWrap(Graphic::TextWrap wrap) -> UIStyleDef& {
        text_wrap_mode = wrap;
        return *this;
    }

    // --- Visibility ---

    auto UIStyleDef::InvisibleButOccupySpace(bool option) -> UIStyleDef& {
        invisible_but_occupy_space = option;
        return *this;
    }

    auto UIStyleDef::HandleScroll(bool option) -> UIFluentAPI & {
        handle_scroll = option;
        return *this;
    }
}
