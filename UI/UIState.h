//
// Created by Wangsicong Wei on 2026-01-09.
//

#ifndef UIState_H
#define UIState_H
#include <cstdint>
#include <string>
#include <optional>
#include <vector>
#include "UIFluentAPI.h"
#include <iostream>
namespace Funccia::UI {
    class UIStyle;
    class UIElement;

    enum class UIFlag : uint8_t {
        None  = 0,
        Hovered = 1 << 0,
        Selected= 1 << 1,
        Pressed = 1 << 2
    };
    inline UIFlag operator|(UIFlag a, UIFlag b) {
        return static_cast<UIFlag>(
            static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
        );
    }

    inline UIFlag operator&(UIFlag a, UIFlag b) {
        return static_cast<UIFlag>(
            static_cast<uint8_t>(a) & static_cast<uint8_t>(b)
        );
    }

    inline UIFlag& operator|=(UIFlag& a, UIFlag b) {
        return a = a | b;
    }

    inline UIFlag& operator&=(UIFlag& a, UIFlag b) {
        return a = a & b;
    }

    inline UIFlag operator~(UIFlag a) {
        return static_cast<UIFlag>(~static_cast<uint8_t>(a));
    }

    class UIState {
    public:
        void Set(UIFlag type, UIStyle& ui_style) {
            UIFlag old = m_state;
            m_state |= type;
            if (old != m_state) OnChange(ui_style);

        }
        void Clear(UIFlag type, UIStyle& ui_style) {
            //std::cout << "Clear called, before: " << static_cast<int>(m_state) << std::endl;
            m_state &= ~type;
            //std::cout << "Clear called, after: " << static_cast<int>(m_state) << std::endl;
            OnChange(ui_style);
        }

        void OnChange(UIStyle& ui_style);

        [[nodiscard]] auto Has(UIFlag type) const -> bool {
            return (m_state & type) == type;
        }

        static auto Parse(const std::string& str) -> std::optional<UIFlag> {
            UIFlag result = UIFlag::None;
            bool valid = false;
            std::string token;

            for (char c : str) {
                if (c == ' ') continue;
                if (c == '|') {
                    if (auto f = TokenToFlag(token)) {
                        result |= *f;
                        valid = true;
                    } else {
                        return std::nullopt;  // unknown flag
                    }
                    token.clear();
                } else {
                    token += c;
                }
            }
            if (!token.empty()) {
                if (auto f = TokenToFlag(token)) {
                    result |= *f;
                    valid = true;
                } else {
                    return std::nullopt;
                }
            }

            return valid ? std::optional{result} : std::nullopt;
        }

    private:
        static auto TokenToFlag(const std::string& s) -> std::optional<UIFlag> {
            if (s == "hovered")  return UIFlag::Hovered;
            if (s == "selected") return UIFlag::Selected;
            if (s == "pressed")  return UIFlag::Pressed;
            return std::nullopt;
        }

        UIFlag m_state = UIFlag::None;
    };

    ///IDK what to do with this for now
    class UIToggleState {
    public:
        UIToggleState() = default;
        virtual ~UIToggleState() = default;

        virtual void Toggle() = 0;
        virtual void Update(UIElement* uie,float deltaTime) = 0;
    };
}

#endif //