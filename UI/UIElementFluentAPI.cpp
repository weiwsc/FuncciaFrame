//
// Created by Wangsicong Wei on 2025-09-27.
//
#include "UIElement.h"
namespace Funccia::UI {
    auto UIElement::SetTag(UI::Tag tag) -> UIElement & {
        m_tag = tag;
        return *this;
    }

    auto UIElement::Text(const std::string &text) -> UIElement & {
        m_text = text;
        return *this;
    }

    auto UIElement::InvisibleButOccupySpace(bool option) -> UIElement & {
        m_invisibleButOccupySpace = option;
        return *this;
    }

    auto UIElement::MarginTop(float value) -> UIElement & {
        m_margin.SetTop(value);
        return *this;
    }

    auto UIElement::MarginLeft(float value) -> UIElement & {
        m_margin.SetLeft(value);
        return *this;
    }

    auto UIElement::MarginRight(float value) -> UIElement & {
        m_margin.SetRight(value);
        return *this;
    }

    auto UIElement::MarginBottom(float value) -> UIElement & {
        m_margin.SetBottom(value);
        return *this;
    }

    auto UIElement::Margin(float all_values) -> UIElement & {
        m_margin = {all_values, all_values, all_values, all_values};
        return *this;
    }

    auto UIElement::Margin(float topButton, float leftRight) -> UIElement & {
        m_margin = {topButton, leftRight, topButton, leftRight};
        return *this;
    }

    auto UIElement::Margin(float top, float right, float bottom, float left) -> UIElement & {
        m_margin = {top, right, bottom, left};
        return *this;
    }

    auto UIElement::PaddingTop(float value) -> UIElement & {
        m_padding.SetTop(value);
        return *this;
    }

    auto UIElement::PaddingLeft(float value) -> UIElement & {
        m_padding.SetLeft(value);
        return *this;
    }

    auto UIElement::PaddingRight(float value) -> UIElement & {
        m_padding.SetRight(value);
        return *this;
    }

    auto UIElement::PaddingBottom(float value) -> UIElement & {
        m_padding.SetBottom(value);
        return *this;
    }


    auto UIElement::Padding(float all_values) -> UIElement & {
        m_padding = {all_values, all_values, all_values, all_values};
        return *this;
    }

    auto UIElement::Padding(float topButton, float leftRight) -> UIElement & {
        m_padding = {topButton, leftRight, topButton, leftRight};
        return *this;
    }

    auto UIElement::Padding(float top, float right, float bottom, float left) -> UIElement & {\
        m_padding = {top, right, bottom, left};
        return *this;
    }

    auto UIElement::Background(vec4 color) -> UIElement & {
        m_background = color;
        return *this;
    }

    auto UIElement::TextColor(vec4 color) -> UIElement & {
        m_color = color;
        return *this;
    }

    auto UIElement::BoxShadow(vec2 offSet, float blur, float spread, vec4 color) -> UIElement & {
        m_shadow.shadow_offset = offSet;
        m_shadow.blur_radius = blur;
        m_shadow.spread = spread;
        m_shadow.shadow_color = color;
        return *this;
    }

    auto UIElement::BoxShadowColor(vec4 color) -> UIElement & {
        m_shadow.shadow_color = color;
        return *this;
    }

    auto UIElement::BoxShadowBlur(float blur) -> UIElement & {
        m_shadow.blur_radius = blur;
        return *this;
    }

    auto UIElement::BoxShadowSpread(float spread) -> UIElement & {
        m_shadow.spread = spread;
        return *this;
    }

    auto UIElement::BoxShadowOffset(vec2 offSet) -> UIElement & {
        m_shadow.shadow_offset = offSet;
        return *this;
    }

    auto UIElement::BorderRadius(vec4 radius) -> UIElement & {
        m_border_radius = radius;
        return *this;
    }

    auto UIElement::BorderColor(vec4 color) -> UIElement & {
        m_border_color = color;
        return *this;
    }

    auto UIElement::BorderWidth(vec4 width) -> UIElement & {
        m_border_widths.SetTop(width.x);
        m_border_widths.SetRight(width.y);
        m_border_widths.SetBottom(width.z);
        m_border_widths.SetLeft(width.w);
        return *this;
    }

    auto UIElement::HorizontalFixed(float size) -> UIElement & {
        m_sizing.x_type = SizingType::Fixed;
        m_sizing.x_value = size;
        return *this;
    }

    auto UIElement::HorizontalGrow() -> UIElement & {
        m_sizing.x_type = SizingType::Grow;
        return *this;
    }

    auto UIElement::HorizontalFit() -> UIElement & {
        m_sizing.x_type = SizingType::Fit;
        return *this;
    }

    auto UIElement::VerticalFixed(float size) -> UIElement & {
        m_sizing.y_type = SizingType::Fixed;
        m_sizing.y_value = size;
        return *this;
    }

    auto UIElement::VerticalGrow() -> UIElement & {
        m_sizing.y_type = SizingType::Grow;
        return *this;
    }

    auto UIElement::VerticalFit() -> UIElement & {
        m_sizing.y_type = SizingType::Fit;
        return *this;
    }

    auto UIElement::HorizontalStack() -> UIElement & {
        m_displayAxis = Axis::Horizontal;
        return *this;
    }

    auto UIElement::VerticalStack() -> UIElement & {
        m_displayAxis = Axis::Vertical;
        return *this;
    }
}
