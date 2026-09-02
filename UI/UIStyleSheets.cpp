//
// Created by Wangsicong Wei on 2026-01-11.
//

#include "UIStyleSheets.h"

#include <algorithm>

namespace vva::UI {
    auto UIStyleSheets::ApplyStyle(UIState ui_state, UIStyle& style) -> void {
        if (m_dirty) {
            std::ranges::sort(m_styles, [](const StateStyle& a, const StateStyle& b) {
                return std::popcount(static_cast<uint8_t>(a.state))
                     < std::popcount(static_cast<uint8_t>(b.state));
            });
            m_dirty = false;
        }
        auto tag = style.tag;  // save tag
        style = UIStyle{};
        style.tag = tag;

        m_default_style.applyTo(style);  // apply default directly

        for (const auto& [flag, def] : m_styles) {
            if (ui_state.Has(flag)) {
                def.applyTo(style);  // apply each matching state on top
            }
        }
    }

    auto UIStyleSheets::Append(vva::UI::UIFlag flag, vva::UI::UIStyleDef style) -> void {
        m_styles.emplace_back(flag, std::move(style));
        m_dirty = true;

    }
}
