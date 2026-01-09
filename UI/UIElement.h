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
#include "UIStyle.h"
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
        auto Style() -> UIStyle& {return m_style;}
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

        UIElement* m_parent {nullptr};
        std::vector<std::unique_ptr<UIElement>> m_children;

        UIStyle m_style;


#ifdef FF_UI_LAZY_LAYOUT
        bool m_layoutUpdated {true};
#endif


        //cached by difference passes
        PositionOffset m_border_box_pos {0,0};
        PositionOffset m_border_box_size {0,0};
        vec4 m_global_border_box {0,0,0,0};
        vec4 m_clipping_box {0,0,0,0};
        bool m_culled {false};

        float m_text_grow_size {};

    };
}

#endif //FUNCCIAFRAME_UIELEMENT_H