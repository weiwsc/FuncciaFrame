//
// Created by Wangsicong Wei on 2026-01-11.
//

#ifndef UISTYLEDEFS_H
#define UISTYLEDEFS_H
#include <tuple>
#include <vector>

#include "UIState.h"
#include "UIStyleDef.h"

namespace Funccia::UI {
    struct StateStyle {
        UIFlag state;
        UIStyleDef def;
    };
    class UIStyleSheets {
    public:
        auto ApplyStyle(Funccia::UI::UIState ui_state, Funccia::UI::UIStyle& style) -> void;
        auto Append(Funccia::UI::UIFlag flag, Funccia::UI::UIStyleDef style) -> void;
        auto SetDefaultStyle(UIStyleDef style) -> void {
            m_default_style = std::move(style);
        }
    private:
        std::vector<StateStyle> m_styles;
        UIStyleDef m_default_style {};

        bool m_dirty = false;
    };
}
#endif //UISTYLEDEFS_H