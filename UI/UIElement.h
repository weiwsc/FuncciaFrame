//
// Created by Wangsicong Wei on 2025-09-25.
//

#ifndef FUNCCIAFRAME_UIELEMENT_H
#define FUNCCIAFRAME_UIELEMENT_H

//#define FF_UI_LAZY_LAYOUT
#include <memory>

#include "../graphic/gl/MathTypes.h"
#include <vector>

#include "SideOffset.h"
#include "UITypes.h"
#include "../graphic/gl/TextRenderer.h"


namespace Funccia::Graphic::GL {
    enum class TextWrap;
    class TextRenderer;
    class UIRenderer;
}

namespace Funccia::UI {
    using UIRender = Funccia::Graphic::GL::UIRenderer;
    class Window;

    class UIElement {
        friend class Window;
    public:
        UIElement() = default;

        explicit UIElement(Tag tag);
        virtual ~UIElement() = default;

        [[nodiscard]] float borderBoxStartOnAxis(Axis axis) const;
        [[nodiscard]] float contentBoxStartOnAxis(Axis axis) const;
        [[nodiscard]] float borderBoxEndOnAxis(Axis axis) const;
        [[nodiscard]] auto borderBoxOnAxis(Axis axis) const -> float;
        [[nodiscard]] auto marginBoxOnAxis(Axis axis) const -> float;
        [[nodiscard]] auto contentBoxOnAxis(Axis axis) const -> float;

        //void SetParent(UIElement* parent);
        auto AddChild(std::unique_ptr<UIElement> child)->UIElement*;
        auto AddChild(Tag tag) -> UIElement*;
        //void RemoveChild(UIElement* child);
        //void RemoveAllChildren();

        // ==========> layout algorithms <==============
        auto CalculateFitSizeOnAxis(Axis axis) -> float;
        auto CalculateGrowSizeOnAxis(Axis axis, Graphic::GL::TextRenderer &text_renderer) -> void;
        auto CalculateTextBounds(Axis axis) -> void;
        auto PositionOnAxis(Axis axis, float parent_content_box_start) -> void;
#ifdef FF_UI_LAZY_LAYOUT
        auto FlipLazyLayoutPass() -> void;
#endif

        auto GlobalPositionPass(float parent_content_box_x, float parent_content_box_y)->void;
        auto CullingPass(vec4 parent_clipping_box) -> void;
        auto RenderQueue(UIRender& render, Graphic::GL::TextRenderer &text_renderer, float parent_content_box_x, float parent_content_box_y) -> void;
        //auto TextRenderQueue(Graphic::GL::TextRenderer& render, float parent_content_box_x, float parent_content_box_y) -> void;
        // ==========> layout algorithms ends <==============
        auto HandlePick(vec2 mouse_pos, Window* parent_window) -> bool;

        auto Scale(float size) -> void;
    protected:
        Tag m_tag {Tag::None};

        UIElement* m_parent {nullptr};
        std::vector<std::unique_ptr<UIElement>> m_children;


        SideOffset m_margin {0,0,0,0};
        SideOffset m_padding {0,0,0,0};


        //vec2 m_min_size {0,0};
        //vec2 m_max_size {0,0};

        //SideOffset m_absoluteOffset {0,0,0,0}; //used for absolute positioning, right/up/left/buttom

        vec4 m_background {0,0,0,0}; //color
        vec4 m_color {0,0,0,1}; //text color, no text rendering yet tho

        SideOffset m_border_widths {0,0,0,0};
        vec4 m_border_radius {0,0,0,0};
        vec4 m_border_color {0,0,0,0};

        Axis m_displayAxis {Axis::Vertical};
        Position m_position {Position::Absolute};
        Sizing m_sizing {SizingType::Grow, SizingType::Fit, 0, 0};

        bool m_is_rendered {true};
        bool m_invisibleButOccupySpace {false};
#ifdef FF_UI_LAZY_LAYOUT
        bool m_layoutUpdated {true};
#endif

        Shadow m_shadow {vec4(0,0,0,0), vec2(0,0), 0, 0};

        //cached by difference passes
        PositionOffset m_border_box_pos {0,0};
        PositionOffset m_border_box_size {0,0};
        vec4 m_global_border_box {0,0,0,0};
        vec4 m_clipping_box {0,0,0,0};
        bool m_culled {false};
        Overflow m_overflow {Overflow::ClipByBorderBox};

        std::string m_text {};
        int m_font_size {35};
        Graphic::GL::TextWrap m_text_wrap_mode {Graphic::GL::TextWrap::Character};
        float m_text_grow_size {};




        // =========> fluent api <=============
    public:
        auto MarginTop(float value) -> UIElement&;
        auto MarginLeft(float value) -> UIElement&;
        auto MarginRight(float value) -> UIElement&;
        auto MarginBottom(float value) -> UIElement&;
        auto Margin(float all_values) -> UIElement&;
        auto Margin(float topButton, float leftRight) -> UIElement&;
        auto Margin(float top, float right, float bottom, float left) -> UIElement&;

        auto PaddingTop(float value) -> UIElement&;
        auto PaddingLeft(float value) -> UIElement&;
        auto PaddingRight(float value) -> UIElement&;
        auto PaddingBottom(float value) -> UIElement&;
        auto Padding(float all_values) -> UIElement&;
        auto Padding(float topButton, float leftRight) -> UIElement&;
        auto Padding(float top, float right, float bottom, float left) -> UIElement&;

        auto Background(vec4 color) -> UIElement&;
        auto TextColor(vec4 color) -> UIElement&;

        auto BoxShadow(vec2 offSet, float blur, float spread, vec4 color) -> UIElement&;
        auto BoxShadowColor(vec4 color) -> UIElement&;
        auto BoxShadowBlur(float blur) -> UIElement&;
        auto BoxShadowSpread(float spread) -> UIElement&;
        auto BoxShadowOffset(vec2 offSet) -> UIElement&;

        auto BorderRadius(vec4 radius) -> UIElement&;
        auto BorderColor(vec4 color) -> UIElement&;
        auto BorderWidth(vec4 width) -> UIElement&;

        auto HorizontalFixed(float size) -> UIElement&;
        auto HorizontalGrow() -> UIElement&;
        auto HorizontalFit() -> UIElement&;

        auto VerticalFixed(float size) -> UIElement&;
        auto VerticalGrow() -> UIElement&;
        auto VerticalFit() -> UIElement&;

        auto HorizontalStack() -> UIElement&;
        auto VerticalStack() -> UIElement&;

        auto SetTag(Tag tag) -> UIElement&;

        auto Text(const std::string& text) -> UIElement&;
        auto FontSize(int size) -> UIElement&;
        auto TextWrap(Graphic::GL::TextWrap wrap) -> UIElement&;

        auto InvisibleButOccupySpace(bool option) -> UIElement&;

    };
}

#endif //FUNCCIAFRAME_UIELEMENT_H