//
// Created by Wangsicong Wei on 2025-09-25.
//

#include "UIElement.h"

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
                return m_border_box_pos.GetX() + m_padding.left();
            case Axis::Vertical:
                return m_border_box_pos.GetY() + m_padding.top();
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

    void UIElement::AddChild(std::unique_ptr<UIElement> child) {
        m_children.push_back(std::move(child));
    }

    auto UIElement::CalculateFitSizeOnAxis(Axis axis)->float {

        float totalChildSize = 0;
        float maxSize = 0;

        float padding = m_padding.firstAndSecond(axis);
        float margin = m_margin.firstAndSecond(axis);

        for (auto& child : m_children) {
            float childSize = child->CalculateFitSizeOnAxis(axis);
            totalChildSize += childSize;
            if (childSize > maxSize) { maxSize = childSize; }
        }

        switch (m_sizing.TypeOnAxis(axis)) {
            case SizingType::Fit:
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

    auto UIElement::PositionOnAxis(Axis axis,  float parent_content_box_start) -> void {
        if (!m_is_rendered) return;

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

    auto UIElement::RenderQueue(UIRender &render, float parent_content_box_x, float parent_content_box_y) -> void {
        if (!m_is_rendered) return;
        vec4 borderBox = {
            parent_content_box_x + m_border_box_pos.GetX(),
            parent_content_box_y + m_border_box_pos.GetY(),
            parent_content_box_x + m_border_box_pos.GetX() + m_border_box_size.GetX(),
            parent_content_box_y + m_border_box_pos.GetY() + m_border_box_size.GetY(),
        };


        render.Collect({
            // a_borderBox (4 floats)
            borderBox.x, borderBox.y, borderBox.z, borderBox.w,

            // a_backgroundColor (4 floats)
            m_background.r, m_background.g, m_background.b, m_background.a,

            // a_borderRadius (4 floats)
            m_border_radius.r, m_border_radius.g, m_border_radius.b, m_border_radius.a
        });
        for (auto& child : m_children) {
            child->RenderQueue(render, parent_content_box_x+ contentBoxStartOnAxis(Axis::Horizontal), parent_content_box_y + contentBoxStartOnAxis(Axis::Vertical));
        }
    }
}
