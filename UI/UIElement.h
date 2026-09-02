//
// Created by Wangsicong Wei on 2025-09-25.
//

#ifndef FUNCCIAFRAME_UIELEMENT_H
#define FUNCCIAFRAME_UIELEMENT_H

//#define FF_UI_LAZY_LAYOUT
#include <memory>

#include "../graphic/MathTypes.h"
#include <vector>

#include "SideOffset.h"
#include "UIState.h"
#include "UIStyle.h"
#include "UITypes.h"
#include "../graphic/RendererInterfaces.h"

namespace vva::UI {
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
        auto CalculateGrowSizeOnAxis(Axis axis, gfx::ITextRenderer &text_renderer) -> void;
        auto CalculateTextBounds(Axis axis) -> void;
        auto PositionOnAxis(Axis axis, float parent_content_box_start) -> void;

        //std::string componentName {};
#ifdef FF_UI_LAZY_LAYOUT
        auto FlipLazyLayoutPass() -> void;
#endif

        auto GlobalPositionPass(float parent_content_box_x, float parent_content_box_y)->void;
        auto CullingPass(vec4 parent_clipping_box) -> void;
        auto RenderQueue(gfx::IUiRenderer& render, gfx::ITextRenderer &text_renderer, float parent_content_box_x, float parent_content_box_y, float scale) -> void;
        //auto TextRenderQueue(graphic::ITextRenderer& render, float parent_content_box_x, float parent_content_box_y) -> void;
        // ==========> layout algorithms ends <==============
        auto HandlePick(vec2 mouse_pos, Window* parent_window) -> bool;
        auto HandleHover(vec2 mouse_pos, Window* parent_window) -> bool;
        auto FindScrollHandler(vec2 mouse_pos) -> UIElement*;
        auto Scale(float size) -> void;

        auto Text(const std::string& text) -> void {m_text = text; m_style.text_recalculate_flag = true;}
        auto GetText() -> std::string {return m_text;}

        auto GetState() -> UIState& {return m_state;}
    protected:

        UIElement* m_parent {nullptr};
        std::vector<std::unique_ptr<UIElement>> m_children;

        UIStyle m_style;
        UIState m_state;

        std::string m_text;

#ifdef FF_UI_LAZY_LAYOUT
        bool m_layoutUpdated {true};
#endif


        //cached by difference passes
        PositionOffset m_border_box_pos {0,0};
        PositionOffset m_border_box_size {0,0};
        vec4 m_global_border_box {0,0,0,0};
        vec4 m_clipping_box {0,0,0,0};
        bool m_culled {false};

        vec2 m_scroll {0,0};

        float m_text_grow_size {};

    };
}

#endif //FUNCCIAFRAME_UIELEMENT_H
