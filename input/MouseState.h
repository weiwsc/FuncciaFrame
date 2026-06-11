//
// Created by Wangsicong Wei on 2026-01-08.
//

#ifndef MOUSESTATE_H
#define MOUSESTATE_H

#include <glm/vec2.hpp>

namespace Funccia::Input {
    enum class MouseButton {
        Left,
        Middle,
        Right
    };

    class MouseState {
    public:
        MouseState() = default;
        virtual ~MouseState() = default;

        [[nodiscard]] auto GetPos() const -> glm::vec2 { return m_pos; }
        [[nodiscard]] auto GetPosPrev() const -> glm::vec2 { return m_posPrev; }
        [[nodiscard]] auto GetButLDown() const -> bool { return m_butLDown; }
        [[nodiscard]] auto GetButMDown() const -> bool { return m_butMDown; }
        [[nodiscard]] auto GetButRDown() const -> bool { return m_butRDown; }
        [[nodiscard]] auto GetScroll() const -> glm::vec2 { return m_scroll; }
        [[nodiscard]] auto HasScrolled() const -> bool { return m_scrolled; }

        auto SetPosition(glm::vec2 pos) -> void;
        auto SetButton(MouseButton button, bool down) -> void;
        auto SetScroll(glm::vec2 scroll) -> void;
        auto ScrollHandled() -> void { m_scrolled = false; }

    private:
        glm::vec2 m_pos {0, 0};
        glm::vec2 m_posPrev {0, 0};
        glm::vec2 m_scroll {0, 0};
        bool m_scrolled {false};
        bool m_butLDown {false};
        bool m_butMDown {false};
        bool m_butRDown {false};
    };
}

#endif //
