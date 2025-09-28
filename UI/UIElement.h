//
// Created by Wangsicong Wei on 2025-09-25.
//

#ifndef FUNCCIAFRAME_UIELEMENT_H
#define FUNCCIAFRAME_UIELEMENT_H
#include <memory>

#include "../graphic/gl/MathTypes.h"
#include <vector>

#include "SideOffset.h"
#include "UITypes.h"
#include "../graphic/gl/UIRender.h"

namespace Funccia::UI {
    using UIRender = Funccia::Graphic::GL::UIRenderer;


    class UIElement {
        friend class Window;
    public:
        UIElement() = default;
        virtual ~UIElement() = default;

        [[nodiscard]] float borderBoxStartOnAxis(Axis axis) const;
        [[nodiscard]] float contentBoxStartOnAxis(Axis axis) const;
        [[nodiscard]] float borderBoxEndOnAxis(Axis axis) const;
        [[nodiscard]] auto borderBoxOnAxis(Axis axis) const -> float;
        [[nodiscard]] auto marginBoxOnAxis(Axis axis) const -> float;
        [[nodiscard]] auto contentBoxOnAxis(Axis axis) const -> float;

        //void SetParent(UIElement* parent);
        void AddChild(std::unique_ptr<UIElement> child);
        //void RemoveChild(UIElement* child);
        //void RemoveAllChildren();

        auto CalculateFitSizeOnAxis(Axis axis) -> float;
        auto CalculateGrowSizeOnAxis(Axis axis) -> void;
        auto PositionOnAxis(Axis axis, float parent_content_box_start) -> void;
        auto RenderQueue(UIRender& render, float parent_content_box_x, float parent_content_box_y) -> void;

        auto Scale(float size) -> void;
    protected:
        UIElement* m_parent {nullptr};
        std::vector<std::unique_ptr<UIElement>> m_children;


        SideOffset m_margin {0,0,0,0};
        SideOffset m_padding {0,0,0,0};


        vec2 m_min_size {0,0};
        vec2 m_max_size {0,0};

        SideOffset m_absoluteOffset {0,0,0,0}; //used for absolute positioning, right/up/left/buttom

        vec4 m_background {0,0,0,0}; //color
        vec4 m_color {0,0,0,0}; //text color, no text rendering yet tho

        SideOffset m_border_widths {0,0,0,0};
        vec4 m_border_radius {0,0,0,0};
        vec4 m_border_color {0,0,0,0};

        Axis m_displayAxis {Axis::Horizontal};
        Position m_position {Position::Absolute};
        Sizing m_sizing {SizingType::Fit, SizingType::Fit, 0, 0};
        bool m_is_rendered {true};

        Shadow m_shadow {vec4(0,0,0,0), vec2(0,0), 0, 0};

        //cached by difference passes
        PositionOffset m_border_box_pos {0,0};
        PositionOffset m_border_box_size {0,0};

    };
}

#endif //FUNCCIAFRAME_UIELEMENT_H