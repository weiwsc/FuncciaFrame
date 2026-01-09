//
// Created by Wangsicong Wei on 2025-09-25.
//

#include "UIElement.h"
#include "../graphic/gl/UIRender.h"
#include "../graphic/gl/TextRenderer.h"
#include <algorithm>
#include <numeric>

#include "UIHelper.h"
#include "Window.h"

namespace Funccia::UI {
    float UIElement::borderBoxStartOnAxis(Axis axis) const {
        switch (axis) {
            case Axis::Horizontal:
                return m_border_box_pos.GetX();
            case Axis::Vertical:
                return m_border_box_pos.GetY();
            default:
                return 0;
        }
    }

    float UIElement::contentBoxStartOnAxis(Axis axis) const {
        switch (axis) {
            case Axis::Horizontal:
                return m_border_box_pos.GetX() + m_padding.left() + m_border_widths.left();
            case Axis::Vertical:
                return m_border_box_pos.GetY() + m_padding.top() + m_border_widths.top();
            default:
                return 0;
        }

    }

    float UIElement::borderBoxEndOnAxis(Axis axis) const {
        switch (axis) {
            case Axis::Horizontal:
                return m_border_box_pos.GetX() + m_border_box_size.GetX();
            case Axis::Vertical:
                return m_border_box_pos.GetY() + m_border_box_size.GetY();
            default:
                return 0;
        }
    }

    auto UIElement::borderBoxOnAxis(Axis axis) const -> float {
        switch (axis) {
            case Axis::Horizontal:
                return m_border_box_size.GetX();
            case Axis::Vertical:
                return m_border_box_size.GetY();
            default:
                return 0;
        }
    }

    auto UIElement::marginBoxOnAxis(Axis axis) const -> float {
        switch (axis) {
            case Axis::Horizontal:
                return m_border_box_size.GetX() + m_margin.left() + m_margin.right();
            case Axis::Vertical:
                return m_border_box_size.GetY() + m_margin.top() + m_margin.bottom();
            default:
        }
    }

    auto UIElement::contentBoxOnAxis(Axis axis) const -> float {
        switch (axis) {
            case Axis::Horizontal:
                return m_border_box_size.GetX() - m_padding.left() - m_padding.right() - m_border_widths.left() - m_border_widths.right();
            case Axis::Vertical:
                return m_border_box_size.GetY() - m_padding.top() - m_padding.bottom() - m_border_widths.top() - m_border_widths.bottom();
        }
    }

    auto UIElement::AddChild(std::unique_ptr<UIElement> child)->UIElement* {
        child->m_parent = this;
        m_children.emplace_back(std::move(child));
        return m_children.back().get(); // raw, non-owning
    }

    auto UIElement::CalculateFitSizeOnAxis(Axis axis)->float {
        if (!m_is_rendered) return 0;
#ifdef FF_UI_LAZY_LAYOUT
        if (!m_layoutUpdated) return m_border_box_size.OnAxis(axis) + m_margin.firstAndSecond(axis);
#endif

        float totalChildSize = 0;
        float maxSize = 0;
        if (axis == Axis::Vertical && !m_text.empty()) {
            totalChildSize = m_text_grow_size;
            maxSize = m_text_grow_size;
        }
        float padding = m_padding.firstAndSecond(axis) + m_border_widths.firstAndSecond(axis);
        float margin = m_margin.firstAndSecond(axis);

        for (auto& child : m_children) {
            float childSize = child->CalculateFitSizeOnAxis(axis);
            totalChildSize += childSize;
            if (childSize > maxSize) { maxSize = childSize; }
        }

        switch (m_sizing.TypeOnAxis(axis)) {
            case SizingType::Fit:
            case SizingType::Grow: //treat grow as fit for now
                if (m_displayAxis == axis) {
                    m_border_box_size.Set(axis, totalChildSize + padding);
                }
                else {
                    m_border_box_size.Set(axis, maxSize + padding); //the size perpendicular to the display axis is the biggest child
                }
                break;
            case SizingType::Fixed:
                m_border_box_size.Set(axis, m_sizing.ValueOnAxis(axis));
                break;
            default:
                m_border_box_size.Set(axis, 0);
        }

        return m_border_box_size.OnAxis(axis) + margin;
    }

    auto UIElement::CalculateGrowSizeOnAxis(Axis axis, Graphic::GL::TextRenderer &text_renderer) -> void {
        if (!m_is_rendered) return;
#ifdef FF_UI_LAZY_LAYOUT
        if (!m_layoutUpdated) return;
#endif

        if (m_children.empty() && !m_text.empty() && axis == Axis::Horizontal) {
            float w = m_border_box_size.GetX()
                              - m_border_widths.left() - m_border_widths.right()
                              - m_padding.left() - m_padding.right();
            m_text_grow_size = text_renderer.ProcessText(0,0, w, m_text_wrap_mode, {0,0,0,0}, m_text, m_color, static_cast<float>(m_font_size), "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/fonts/NotoSans-VariableFont_wdth,wght.ttf" );
        }

        std::vector<UIElement*> childToGrow {};
        float usedSpace = 0;

        // Count ALL children's space, but track which ones can grow
        for (auto& child : m_children) {
            if (child->m_sizing.TypeOnAxis(axis) == SizingType::Grow) {
                childToGrow.push_back(child.get());
            }
            // Always add to usedSpace - grow children have initial size from Fit pass
            usedSpace += child->marginBoxOnAxis(axis);
        }

        float availableSpace = contentBoxOnAxis(axis) - usedSpace;

        if (m_displayAxis != axis) {
            // Cross-axis: grow children fill the cross dimension
            for (auto& child : childToGrow) {
                child->m_border_box_size.Set(axis, contentBoxOnAxis(axis) - child->m_margin.firstAndSecond(axis));
            }
        }
        else if (availableSpace > 0 && !childToGrow.empty()) {
            // Main axis: distribute extra space equally among grow children
            float spacePerChild = availableSpace / static_cast<float>(childToGrow.size());

            for (auto& child : childToGrow) {
                float currentSize = child->m_border_box_size.OnAxis(axis);
                child->m_border_box_size.Set(axis, currentSize + spacePerChild);
            }
        }

        // Recurse to children
        for (auto& child : m_children) {
            child->CalculateGrowSizeOnAxis(axis, text_renderer);
        }
    }

    auto UIElement::CalculateTextBounds(Axis axis) -> void {
        //do I need this? maybe not for now
    }

    auto UIElement::PositionOnAxis(Axis axis,  float parent_content_box_start) -> void {
        if (!m_is_rendered) return;
#ifdef FF_UI_LAZY_LAYOUT
        if (!m_layoutUpdated) return;
#endif
        m_border_box_pos.Set(axis, parent_content_box_start + m_margin.first(axis));
        //this should be 0, since the origin is the start of the parent's content box IN THE PERSPECTIVE OF THE CHILDREN ELEMENTS
        float cursorPos = 0;

        bool isOnAxis = (m_displayAxis == axis);

        for (auto& child : m_children) {
            child->PositionOnAxis(axis, cursorPos);
            float childSize = child->m_border_box_size.OnAxis(axis) + child->m_margin.firstAndSecond(axis);
            if (isOnAxis) {
                cursorPos += childSize;
            }
        }
    }
#ifdef FF_UI_LAZY_LAYOUT
    auto UIElement::FlipLazyLayoutPass() -> void {
        if (!m_is_rendered) return;
        m_layoutUpdated = false;
        for (auto& child : m_children) {
            child->FlipLazyLayoutPass();
        }
    }
#endif
    auto UIElement::GlobalPositionPass(float parent_content_box_x, float parent_content_box_y) -> void {
        if (!m_is_rendered) return;
        m_global_border_box = {
            parent_content_box_x + m_border_box_pos.GetX(),
            parent_content_box_y + m_border_box_pos.GetY(),
            parent_content_box_x + m_border_box_pos.GetX() + m_border_box_size.GetX(),
            parent_content_box_y + m_border_box_pos.GetY() + m_border_box_size.GetY(),
        };
        for (auto& child : m_children) {
            child->GlobalPositionPass(parent_content_box_x + contentBoxStartOnAxis(Axis::Horizontal), parent_content_box_y + contentBoxStartOnAxis(Axis::Vertical));
        }
    }

    auto UIElement::CullingPass(vec4 parent_clipping_box) -> void {
        if (!m_is_rendered) return;
        m_culled = false;


        vec4 border_clipping_box = UIHelper::intersect(parent_clipping_box, m_global_border_box);

        if (border_clipping_box.x > border_clipping_box.z || border_clipping_box.y > border_clipping_box.w) {
            m_culled = true;
            return;
        }
        m_clipping_box = parent_clipping_box;

        vec4 content_culling_box = parent_clipping_box;
        switch (m_overflow) {
            case Overflow::Visible:
                break;
            case Overflow::ClipByBorderBox:
                content_culling_box = border_clipping_box;
                break;
        }
        content_culling_box.x += m_border_widths.left();
        content_culling_box.y += m_border_widths.top();
        content_culling_box.z -= m_border_widths.right();   //without cutting the border width, the clipped child will overlap the parent's border. if the border box calculated is changed, PLS CHANGE THIS AS WELL!!!!!!
        content_culling_box.w -= m_border_widths.bottom();
        for (auto& child : m_children) {
            child->CullingPass(content_culling_box);
        }
    }


    auto UIElement::RenderQueue(UIRender &render, Graphic::GL::TextRenderer &text_renderer,float parent_content_box_x, float parent_content_box_y) -> void {
        if (m_invisibleButOccupySpace || !m_is_rendered || m_culled) {return;}
        // vec4 borderBox = {
        //     parent_content_box_x + m_border_box_pos.GetX(),
        //     parent_content_box_y + m_border_box_pos.GetY(),
        //     parent_content_box_x + m_border_box_pos.GetX() + m_border_box_size.GetX(),
        //     parent_content_box_y + m_border_box_pos.GetY() + m_border_box_size.GetY(),
        // };
        vec4 borderColor;
        if (m_border_color.r == 0 && m_border_color.g == 0 && m_border_color.b == 0 && m_border_color.a == 0) {
            borderColor.r = m_background.r;
            borderColor.g = m_background.g;
            borderColor.b = m_background.b;
            borderColor.a = m_background.a;
        }
        else {
            borderColor = m_border_color;
        }

        render.Collect({
            // a_borderBox (4 floats)
            m_global_border_box.x, m_global_border_box.y, m_global_border_box.z, m_global_border_box.w,
            // a_backgroundColor (4 floats)
            m_background.r, m_background.g, m_background.b, m_background.a,
            // a_borderRadius (4 floats)
            m_border_radius.r, m_border_radius.g, m_border_radius.b, m_border_radius.a,
            // a_shadowProperties (4 floats)
            m_shadow.shadow_offset.x, m_shadow.shadow_offset.y, m_shadow.blur_radius, m_shadow.spread,
            // a_shadowColor (4 floats)
            m_shadow.shadow_color.r, m_shadow.shadow_color.g, m_shadow.shadow_color.b, m_shadow.shadow_color.a,
            // a_borderWidths (4 floats)
            m_border_widths.top(), m_border_widths.right(), m_border_widths.bottom(), m_border_widths.left(),
            // a_borderColor (4 floats)
            borderColor.r, borderColor.g, borderColor.b, borderColor.a,
            // the clipping box (4 floats)
            m_clipping_box.x, m_clipping_box.y, m_clipping_box.z, m_clipping_box.w,
        });

        if (!m_text.empty()) {
            // Calculate content box position from the global border box
            float content_x = m_global_border_box.x + m_border_widths.left() + m_padding.left();
            float content_y = m_global_border_box.y + m_border_widths.top() + m_padding.top();
            float content_w = (m_global_border_box.z - m_global_border_box.x)
                              - m_border_widths.left() - m_border_widths.right()
                              - m_padding.left() - m_padding.right();

            text_renderer.ProcessText(content_x,
                content_y,
                content_w,
                m_text_wrap_mode,
                m_clipping_box,
                m_text,
                m_color,
                static_cast<float>(m_font_size),
                "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/fonts/NotoSans-VariableFont_wdth,wght.ttf",
                false
                );
        }

        for (auto& child : m_children) {
            child->RenderQueue(render,text_renderer, parent_content_box_x+ contentBoxStartOnAxis(Axis::Horizontal), parent_content_box_y + contentBoxStartOnAxis(Axis::Vertical));
        }
    }

    auto UIElement::HandlePick(vec2 mouse_pos, Window* parent_window) -> bool {
        if (!m_is_rendered || m_culled) { return false; }

        if (UIHelper::point_intersect(m_global_border_box, mouse_pos)) {
            for (auto& child : m_children) {
                if (child->HandlePick(mouse_pos, parent_window)) {
                    return true;  // child handled it, we're done
                }
            }
            // no child handled it
            parent_window->selected_element = this;
            return true;
        }

        return false;
    }


    auto UIElement::Scale(float size) -> void {
        if (m_sizing.x_type == SizingType::Fixed) {
            m_sizing.x_value *= size;
        }
        if (m_sizing.y_type == SizingType::Fixed) {
            m_sizing.y_value *= size;
        }
        m_border_radius *= size;
        m_border_widths.SetTop(m_border_widths.top() * size);
        m_border_widths.SetRight(m_border_widths.right() * size);
        m_border_widths.SetBottom(m_border_widths.bottom() * size);
        m_border_widths.SetLeft(m_border_widths.left() * size);
        for (auto& child : m_children) {
            child->Scale(size);
        }
    }
}
