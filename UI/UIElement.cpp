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
#include "../graphic/WindowController.h"
#include "../core/ProfileTimer.h"
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
                return m_border_box_pos.GetX() + m_style.padding.left() + m_style.border_widths.left();
            case Axis::Vertical:
                return m_border_box_pos.GetY() + m_style.padding.top() + m_style.border_widths.top();
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
                return m_border_box_size.GetX() + m_style.margin.left() + m_style.margin.right();
            case Axis::Vertical:
                return m_border_box_size.GetY() + m_style.margin.top() + m_style.margin.bottom();
            default:

        }
    }

    auto UIElement::contentBoxOnAxis(Axis axis) const -> float {
        switch (axis) {
            case Axis::Horizontal:
                return m_border_box_size.GetX() - m_style.padding.left() - m_style.padding.right() - m_style.
                       border_widths.left() - m_style.border_widths.right();
            case Axis::Vertical:
                return m_border_box_size.GetY() - m_style.padding.top() - m_style.padding.bottom() - m_style.
                       border_widths.top() - m_style.border_widths.bottom();
        }
    }

    auto UIElement::AddChild(std::unique_ptr<UIElement> child) -> UIElement * {
        child->m_parent = this;
        m_children.emplace_back(std::move(child));
        return m_children.back().get(); // raw, non-owning
    }

    auto UIElement::CalculateFitSizeOnAxis(Axis axis) -> float {
        if (!m_style.is_rendered) return 0;
#ifdef FF_UI_LAZY_LAYOUT
        if (!m_layoutUpdated) return m_border_box_size.OnAxis(axis) + m_style.margin.firstAndSecond(axis);
#endif

        float totalChildSize = 0;
        float maxSize = 0;
        if (axis == Axis::Vertical && !m_text.empty()) {
            totalChildSize = m_text_grow_size;
            maxSize = m_text_grow_size;
        }
        float padding = m_style.padding.firstAndSecond(axis) + m_style.border_widths.firstAndSecond(axis);
        float margin = m_style.margin.firstAndSecond(axis);

        for (auto &child: m_children) {
            float childSize = child->CalculateFitSizeOnAxis(axis);
            totalChildSize += childSize;
            if (childSize > maxSize) { maxSize = childSize; }
        }

        switch (m_style.sizing.TypeOnAxis(axis)) {
            case SizingType::Fit:
            case SizingType::Grow: //treat grow as fit for now
                if (m_style.display_axis == axis) {
                    m_border_box_size.Set(axis, totalChildSize + padding);
                } else {
                    m_border_box_size.Set(axis, maxSize + padding);
                    //the size perpendicular to the display axis is the biggest child
                }
                break;
            case SizingType::Fixed:
                m_border_box_size.Set(axis, m_style.sizing.ValueOnAxis(axis));
                break;
            default:
                m_border_box_size.Set(axis, 0);
        }

        return m_border_box_size.OnAxis(axis) + margin;
    }

    auto UIElement::CalculateGrowSizeOnAxis(Axis axis, Graphic::GL::TextRenderer &text_renderer) -> void {
        if (!m_style.is_rendered) return;
#ifdef FF_UI_LAZY_LAYOUT
        if (!m_layoutUpdated) return;
#endif

        { //PROFILE_SCOPE("calculate text");
            if (m_style.text_recalculate_flag && m_children.empty() && !m_text.empty() && axis == Axis::Horizontal) {
                float w = m_border_box_size.GetX()
                          - m_style.border_widths.left() - m_style.border_widths.right()
                          - m_style.padding.left() - m_style.padding.right();
                m_text_grow_size = text_renderer.ProcessText(0, 0, w, m_style.text_wrap_mode, {0, 0, 0, 0}, m_text,
                                                             m_style.color, static_cast<float>(m_style.font_size),
                                                             m_style.font_path);
                m_style.text_recalculate_flag = false;
            }
        }
        std::vector<UIElement *> childToGrow{};
        float usedSpace = 0;

        // Count ALL children's space, but track which ones can grow
        for (auto &child: m_children) {
            if (child->m_style.sizing.TypeOnAxis(axis) == SizingType::Grow) {
                childToGrow.push_back(child.get());
            }
            // Always add to usedSpace - grow children have initial size from Fit pass
            usedSpace += child->marginBoxOnAxis(axis);
        }

        float availableSpace = contentBoxOnAxis(axis) - usedSpace;

        if (m_style.display_axis != axis) {
            // Cross-axis: grow children fill the cross dimension
            for (auto &child: childToGrow) {
                child->m_border_box_size.Set(axis, contentBoxOnAxis(axis) - child->m_style.margin.firstAndSecond(axis));
            }
        } else if (availableSpace > 0 && !childToGrow.empty()) {
            // Main axis: distribute extra space equally among grow children
            float spacePerChild = availableSpace / static_cast<float>(childToGrow.size());

            for (auto &child: childToGrow) {
                float currentSize = child->m_border_box_size.OnAxis(axis);
                child->m_border_box_size.Set(axis, currentSize + spacePerChild);
            }
        }

        // Recurse to children
        for (auto &child: m_children) {
            child->CalculateGrowSizeOnAxis(axis, text_renderer);
        }
    }

    auto UIElement::CalculateTextBounds(Axis axis) -> void {
        //do I need this? maybe not for now
    }

    auto UIElement::PositionOnAxis(Axis axis, float parent_content_box_start) -> void {
        if (!m_style.is_rendered) return;
#ifdef FF_UI_LAZY_LAYOUT
        if (!m_layoutUpdated) return;
#endif
        m_border_box_pos.Set(axis, parent_content_box_start + m_style.margin.first(axis));
        //this should be 0, since the origin is the start of the parent's content box IN THE PERSPECTIVE OF THE CHILDREN ELEMENTS
        float cursorPos = 0;

        bool isOnAxis = (m_style.display_axis == axis);

        for (auto &child: m_children) {
            child->PositionOnAxis(axis, cursorPos);
            float childSize = child->m_border_box_size.OnAxis(axis) + child->m_style.margin.firstAndSecond(axis);
            if (isOnAxis) {
                cursorPos += childSize;
            }
        }
    }
#ifdef FF_UI_LAZY_LAYOUT
    auto UIElement::FlipLazyLayoutPass() -> void {
        if (!m_style.is_rendered) return;
        m_layoutUpdated = false;
        for (auto &child: m_children) {
            child->FlipLazyLayoutPass();
        }
    }
#endif
    auto UIElement::GlobalPositionPass(float parent_content_box_x, float parent_content_box_y) -> void {
        if (!m_style.is_rendered) return;
        m_global_border_box = {
            parent_content_box_x + m_border_box_pos.GetX(),
            parent_content_box_y + m_border_box_pos.GetY(),
            parent_content_box_x + m_border_box_pos.GetX() + m_border_box_size.GetX(),
            parent_content_box_y + m_border_box_pos.GetY() + m_border_box_size.GetY(),
        };
        for (auto &child: m_children) {
            child->GlobalPositionPass(parent_content_box_x + contentBoxStartOnAxis(Axis::Horizontal) + m_scroll.x,
                                      parent_content_box_y + contentBoxStartOnAxis(Axis::Vertical) + m_scroll.y);
        }
    }

    auto UIElement::CullingPass(vec4 parent_clipping_box) -> void {
        if (!m_style.is_rendered) return;
        m_culled = false;


        vec4 border_clipping_box = UIHelper::intersect(parent_clipping_box, m_global_border_box);

        if (border_clipping_box.x > border_clipping_box.z || border_clipping_box.y > border_clipping_box.w) {
            m_culled = true;
            return;
        }
        m_clipping_box = parent_clipping_box;

        vec4 content_culling_box = parent_clipping_box;
        switch (m_style.overflow) {
            case Overflow::Visible:
                break;
            case Overflow::ClipByBorderBox:
                content_culling_box = border_clipping_box;
                break;
        }
        content_culling_box.x += m_style.border_widths.left();
        content_culling_box.y += m_style.border_widths.top();
        content_culling_box.z -= m_style.border_widths.right();
        //without cutting the border width, the clipped child will overlap the parent's border. if the border box calculated is changed, PLS CHANGE THIS AS WELL!!!!!!
        content_culling_box.w -= m_style.border_widths.bottom();
        for (auto &child: m_children) {
            child->CullingPass(content_culling_box);
        }
    }


    auto UIElement::RenderQueue(UIRender &render, Graphic::GL::TextRenderer &text_renderer, float parent_content_box_x,
                                float parent_content_box_y) -> void {
        float s = Graphic::GL::WindowController::Instance().GetWindow()->GetDisplayScale();
        if (m_style.invisible_but_occupy_space || !m_style.is_rendered || m_culled) { return; }
        // vec4 borderBox = {
        //     parent_content_box_x + m_border_box_pos.GetX(),
        //     parent_content_box_y + m_border_box_pos.GetY(),
        //     parent_content_box_x + m_border_box_pos.GetX() + m_border_box_size.GetX(),
        //     parent_content_box_y + m_border_box_pos.GetY() + m_border_box_size.GetY(),
        // };
        vec4 borderColor;
        if (m_style.border_color.r == 0 && m_style.border_color.g == 0 && m_style.border_color.b == 0 && m_style.
            border_color.a == 0) {
            borderColor.r = m_style.background.r;
            borderColor.g = m_style.background.g;
            borderColor.b = m_style.background.b;
            borderColor.a = m_style.background.a;
        } else {
            borderColor = m_style.border_color;
        }

        render.Collect({
            // a_borderBox (4 floats)
            m_global_border_box.x * s, m_global_border_box.y * s, m_global_border_box.z * s, m_global_border_box.w * s,
            // a_backgroundColor (4 floats)
            m_style.background.r, m_style.background.g, m_style.background.b, m_style.background.a,
            // a_borderRadius (4 floats)
            m_style.border_radius.r * s, m_style.border_radius.g * s, m_style.border_radius.b * s,
            m_style.border_radius.a * s,
            // a_shadowProperties (4 floats)
            m_style.shadow.shadow_offset.x * s, m_style.shadow.shadow_offset.y * s, m_style.shadow.blur_radius * s,
            m_style.shadow.spread * s,
            // a_shadowColor (4 floats)
            m_style.shadow.shadow_color.r, m_style.shadow.shadow_color.g, m_style.shadow.shadow_color.b,
            m_style.shadow.shadow_color.a,
            // a_borderWidths (4 floats)
            m_style.border_widths.top() * s, m_style.border_widths.right() * s, m_style.border_widths.bottom() * s,
            m_style.border_widths.left() * s,
            // a_borderColor (4 floats)
            borderColor.r, borderColor.g, borderColor.b, borderColor.a,
            // the clipping box (4 floats)
            m_clipping_box.x * s, m_clipping_box.y * s, m_clipping_box.z * s, m_clipping_box.w * s,
        });

        if (!m_text.empty()) {
            // Calculate content box position from the global border box
            float content_x = m_global_border_box.x + m_style.border_widths.left() + m_style.padding.left();
            float content_y = m_global_border_box.y + m_style.border_widths.top() + m_style.padding.top();
            float content_w = (m_global_border_box.z - m_global_border_box.x)
                              - m_style.border_widths.left() - m_style.border_widths.right()
                              - m_style.padding.left() - m_style.padding.right();

            text_renderer.ProcessText(
                content_x * s,
                content_y * s,
                content_w * s,
                m_style.text_wrap_mode,
                vec4(m_clipping_box.x * s, m_clipping_box.y * s,
                     m_clipping_box.z * s, m_clipping_box.w * s),
                m_text,
                m_style.color,
                static_cast<float>(m_style.font_size) * s,
                m_style.font_path,
                false
            );
        }

        for (auto &child: m_children) {
            child->RenderQueue(render, text_renderer, parent_content_box_x + contentBoxStartOnAxis(Axis::Horizontal),
                               parent_content_box_y + contentBoxStartOnAxis(Axis::Vertical));
        }
    }

    auto UIElement::HandlePick(vec2 mouse_pos, Window *parent_window) -> bool {
        if (!m_style.is_rendered || m_culled) { return false; }

        if (UIHelper::point_intersect(m_global_border_box, mouse_pos)) {
            for (auto &child: m_children) {
                if (child->HandlePick(mouse_pos, parent_window)) {
                    return true; // child handled it, we're done
                }
            }
            // no child handled it
            parent_window->selected_element = this;
            return true;
        }

        return false;
    }

    auto UIElement::HandleHover(vec2 mouse_pos, Window *parent_window) -> bool {
        if (!m_style.is_rendered || m_culled) { return false; }

        if (UIHelper::point_intersect(m_global_border_box, mouse_pos)) {
            for (auto &child: m_children) {
                if (child->HandleHover(mouse_pos, parent_window)) {
                    return true; // child handled it, we're done
                }
            }
            // no child handled it
            parent_window->SetHoveredElement(this);
            return true;
        }

        return false;
    }

    auto UIElement::FindScrollHandler(vec2 mouse_pos) -> UIElement * {
        if (!m_style.is_rendered || m_culled) return nullptr;
        if (!UIHelper::point_intersect(m_global_border_box, mouse_pos)) return nullptr;

        // Search children first so deepest element wins (typical UI behavior)
        for (auto& child : m_children) {
            if (UIElement* h = child->FindScrollHandler(mouse_pos)) {
                return h;
            }
        }

        // If no child handled, this element can handle if it wants to
        if (m_style.handle_scroll) return this;
        return nullptr;
    }


    auto UIElement::Scale(float size) -> void {
        if (m_style.sizing.x_type == SizingType::Fixed) {
            m_style.sizing.x_value *= size;
        }
        if (m_style.sizing.y_type == SizingType::Fixed) {
            m_style.sizing.y_value *= size;
        }
        m_style.border_radius *= size;
        m_style.border_widths.SetTop(m_style.border_widths.top() * size);
        m_style.border_widths.SetRight(m_style.border_widths.right() * size);
        m_style.border_widths.SetBottom(m_style.border_widths.bottom() * size);
        m_style.border_widths.SetLeft(m_style.border_widths.left() * size);
        for (auto &child: m_children) {
            child->Scale(size);
        }
    }
}
