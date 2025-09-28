//
// Created by Wangsicong Wei on 2025-09-25.
//

#include "UIElement.h"

#include <algorithm>
#include <numeric>

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

    void UIElement::AddChild(std::unique_ptr<UIElement> child) {
        m_children.push_back(std::move(child));
    }

    auto UIElement::CalculateFitSizeOnAxis(Axis axis)->float {

        float totalChildSize = 0;
        float maxSize = 0;

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

    auto UIElement::CalculateGrowSizeOnAxis(Axis axis) -> void {
        if (!m_is_rendered) return;
        std::vector<UIElement*> childToGrow {};

        float usedSpace = 0;

        for (auto& child : m_children) {
            if (child->m_sizing.TypeOnAxis(axis) == SizingType::Grow) { childToGrow.push_back(child.get()); }
            usedSpace += child->marginBoxOnAxis(axis);
        }

        float availableSpace = std::max(contentBoxOnAxis(axis) - usedSpace, 0.0f);

        if (m_displayAxis != axis) {
            for (auto& child : childToGrow) {
                child->m_border_box_size.Set(axis, contentBoxOnAxis(axis) - child->m_margin.firstAndSecond(axis));
            }
        }
        else if (availableSpace > 0){ // if we are going along the axis
            if (childToGrow.size() == 1) {
                childToGrow[0]->m_border_box_size.Set(axis, contentBoxOnAxis(axis) - childToGrow[0]->m_margin.firstAndSecond(axis));
            }
            else if (!childToGrow.empty()) { //if there are multiple children to be resized
                float totalWeight = 0;
                std::vector<float> weights;

                for (auto& child : childToGrow) {
                    float currentSize = child->m_border_box_size.OnAxis(axis);
                    // Smaller elements get higher weight (inverse relationship)
                    float weight = 1.0f / std::max(currentSize, 1.0f); // Avoid division by zero
                    weights.push_back(weight);
                    totalWeight += weight;
                }

                // Distribute space based on weights
                for (size_t i = 0; i < childToGrow.size(); ++i) {
                    float weightRatio = weights[i] / totalWeight;
                    float additionalSpace = availableSpace * weightRatio;
                    float currentSize = childToGrow[i]->m_border_box_size.OnAxis(axis);
                    childToGrow[i]->m_border_box_size.Set(axis, currentSize + additionalSpace);
                }
            }
        }

        for (auto& child : m_children) {
            child->CalculateGrowSizeOnAxis(axis);
        }
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
        vec4 borderColor;
        if (m_border_color.r == 0 ,m_border_color.g == 0, m_border_color.b == 0, m_border_color.a == 0) {
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
            borderBox.x, borderBox.y, borderBox.z, borderBox.w,
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
            borderColor.r, borderColor.g, borderColor.b, borderColor.a
        });

        for (auto& child : m_children) {
            child->RenderQueue(render, parent_content_box_x+ contentBoxStartOnAxis(Axis::Horizontal), parent_content_box_y + contentBoxStartOnAxis(Axis::Vertical));
        }
    }

    auto UIElement::Scale(float size) -> void {
        if (m_sizing.x_type == SizingType::Fixed) {
            m_sizing.x_value *= size;
        }
        if (m_sizing.y_type == SizingType::Fixed) {
            m_sizing.y_value *= size;
        }
        m_border_radius *= size;
        //m_border_widths.SetTop(m_border_widths.top() * size);
        //m_border_widths.SetRight(m_border_widths.right() * size);
        //m_border_widths.SetBottom(m_border_widths.bottom() * size);
        //m_border_widths.SetLeft(m_border_widths.left() * size);
        for (auto& child : m_children) {
            child->Scale(size);
        }
    }
}
