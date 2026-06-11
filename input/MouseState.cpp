//
// Created by Wangsicong Wei on 2026-01-08.
//

#include "MouseState.h"

namespace Funccia::Input {
    auto MouseState::SetPosition(glm::vec2 pos) -> void {
        m_posPrev = m_pos;
        m_pos = pos;
    }

    auto MouseState::SetButton(MouseButton button, bool down) -> void {
        switch (button) {
            case MouseButton::Left:
                m_butLDown = down;
                break;
            case MouseButton::Middle:
                m_butMDown = down;
                break;
            case MouseButton::Right:
                m_butRDown = down;
                break;
        }
    }

    auto MouseState::SetScroll(glm::vec2 scroll) -> void {
        m_scroll = scroll;
        m_scrolled = true;
    }
}
