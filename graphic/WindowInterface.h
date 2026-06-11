//
// Created by Wangsicong Wei on 2025-10-05.
//

#ifndef FUNCCIAFRAME_INTERFACE_H
#define FUNCCIAFRAME_INTERFACE_H
#include <string>

#include "RenderBackend.h"

namespace Funccia::Graphic {
    enum class Key;
}

namespace Funccia::Input {
    class MouseState;
}

namespace Funccia::Graphic {
    class WindowInterface{
        public:
        virtual ~WindowInterface() = default;
        auto virtual Initialize(int _width, int _height, std::string _title, RenderBackend backend) -> bool = 0;
        auto virtual Close() -> void = 0;

        auto virtual ShouldClose() -> bool = 0;
        virtual auto SetShouldClose(bool value) -> void = 0;

        // Frame operations
        virtual auto PollEvents() -> void = 0;
        virtual auto SwapBuffers() -> void = 0;

        // Window properties
        virtual auto GetFramebufferSize(int &width, int &height) const -> void = 0;
        virtual auto SetTitle(const char *title) -> void = 0;

        // Input queries
        [[nodiscard]] virtual auto IsKeyPressed(Key key) const -> bool = 0;
        virtual auto GetCursorPos(double &x, double &y) const -> void = 0;

        // Callbacks
        //virtual void SetCursorPosCallback(void* userPtr, void(*callback)(void*, double, double)) = 0;

        virtual void* GetNativeWindow() = 0;

        virtual auto GetDisplayScale()->float = 0;

        [[nodiscard]] virtual auto Mouse()-> Input::MouseState* = 0;

    };

    enum class Key {
        Unknown = -1,

        // Alphanumeric keys
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

        // Function keys
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        // Arrow keys
        Up, Down, Left, Right,

        // Special keys
        Space, Enter, Escape, Tab, Backspace, Delete,
        Insert, Home, End, PageUp, PageDown,

        // Modifiers
        LeftShift, RightShift,
        LeftControl, RightControl,
        LeftAlt, RightAlt,
        LeftSuper, RightSuper, // Windows/Command key

        // Symbols
        Minus, Equal, LeftBracket, RightBracket,
        Backslash, Semicolon, Apostrophe, Comma,
        Period, Slash, GraveAccent,

        CapsLock, ScrollLock, NumLock, PrintScreen, Pause
    };
}

#endif //FUNCCIAFRAME_INTERFACE_H
