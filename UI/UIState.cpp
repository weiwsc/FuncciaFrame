//
// Created by Wangsicong Wei on 2026-01-09.
//

#include "UIState.h"

#include "JsonUiBuilder.h"

void Funccia::UI::UIState::OnChange(UIStyle &ui_style) {
    if (auto it = JsonUiBuilder::styleSheets.find(ui_style.tag); it != JsonUiBuilder::styleSheets.end()) {
        it->second.ApplyStyle(*this, ui_style);
    }
}
