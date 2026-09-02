//
// Created by Wangsicong Wei on 2026-01-09.
//

#ifndef UISTYLEDEF_H
#define UISTYLEDEF_H

#include <optional>
#include <string>
#include "SideOffset.h"
#include "UIFluentAPI.h"
#include "UIStyle.h"

namespace vva::UI {
    class UIStyleDef : public UIFluentAPI {
    public:
        UIStyleDef() = default;
        // Move OK
        UIStyleDef(UIStyleDef&&) = default;
        UIStyleDef& operator=(UIStyleDef&&) = default;

        // Copy deleted
        UIStyleDef(const UIStyleDef&) = delete;
        UIStyleDef& operator=(const UIStyleDef&) = delete;

        std::optional<std::string> tag;

        // Box model
        std::optional<SideOffset> margin;
        std::optional<SideOffset> padding;
        std::optional<SideOffset> border_widths;

        // Sizing & layout
        std::optional<Axis> display_axis;
        std::optional<Position> position;
        std::optional<Sizing> sizing;
        std::optional<Overflow> overflow;

        // Visibility
        std::optional<bool> is_rendered;
        std::optional<bool> invisible_but_occupy_space;

        // Colors
        std::optional<vec4> background;
        std::optional<vec4> color;
        std::optional<vec4> border_color;

        // Border appearance
        std::optional<vec4> border_radius;

        // Shadow
        std::optional<Shadow> shadow;

        // Text
        //std::optional<std::string> text;
        std::optional<int> font_size;
        std::optional<gfx::TextWrap> text_wrap_mode;
        std::optional<std::string> font_path;

        std::optional<bool> handle_scroll;

        // Merge another style def on top of this one (other's values override)
        void merge(const UIStyleDef& other);

        // Apply this def onto a resolved UIStyle
        void applyTo(UIStyle& style) const;

        auto MarginTop(float value) -> UIStyleDef& override;
        auto MarginLeft(float value) -> UIStyleDef& override;
        auto MarginRight(float value) -> UIStyleDef& override;
        auto MarginBottom(float value) -> UIStyleDef& override;
        auto Margin(float all_values) -> UIStyleDef& override;
        auto Margin(float topButton, float leftRight) -> UIStyleDef& override;
        auto Margin(float top, float right, float bottom, float left) -> UIStyleDef& override;

        auto PaddingTop(float value) -> UIStyleDef& override;
        auto PaddingLeft(float value) -> UIStyleDef& override;
        auto PaddingRight(float value) -> UIStyleDef& override;
        auto PaddingBottom(float value) -> UIStyleDef& override;
        auto Padding(float all_values) -> UIStyleDef& override;
        auto Padding(float topButton, float leftRight) -> UIStyleDef& override;
        auto Padding(float top, float right, float bottom, float left) -> UIStyleDef& override;

        auto Background(vec4 _color) -> UIStyleDef& override;
        auto TextColor(vec4 _color) -> UIStyleDef& override;

        auto BoxShadow(vec2 offSet, float blur, float spread, vec4 _color) -> UIStyleDef& override;
        auto BoxShadowColor(vec4 _color) -> UIStyleDef& override;
        auto BoxShadowBlur(float blur) -> UIStyleDef& override;
        auto BoxShadowSpread(float spread) -> UIStyleDef& override;
        auto BoxShadowOffset(vec2 offSet) -> UIStyleDef& override;

        auto BorderRadius(vec4 radius) -> UIStyleDef& override;
        auto BorderColor(vec4 _color) -> UIStyleDef& override;
        auto BorderWidth(vec4 width) -> UIStyleDef& override;

        auto HorizontalFixed(float size) -> UIStyleDef& override;
        auto HorizontalGrow() -> UIStyleDef& override;
        auto HorizontalFit() -> UIStyleDef& override;

        auto VerticalFixed(float size) -> UIStyleDef& override;
        auto VerticalGrow() -> UIStyleDef& override;
        auto VerticalFit() -> UIStyleDef& override;

        auto HorizontalStack() -> UIStyleDef& override;
        auto VerticalStack() -> UIStyleDef& override;

        auto SetTag(const std::string& _tag) -> UIStyleDef& override;

        //auto Text(const std::string& _text) -> UIStyleDef& override;
        auto FontSize(int size) -> UIStyleDef& override;
        auto TextWrap(gfx::TextWrap wrap) -> UIStyleDef& override;

        auto InvisibleButOccupySpace(bool option) -> UIStyleDef& override;

        auto HandleScroll(bool option) -> UIFluentAPI& override;
        ~UIStyleDef() override = default;
    };
}

#endif //UISTYLEDEF_H
