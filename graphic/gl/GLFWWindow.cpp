//
// Created by Wangsicong Wei on 2025-10-05.
//

#include "GLFWWindow.h"

#include <iostream>
/*
namespace Funccia::Graphic::GL {
    GLFWWindow::~GLFWWindow() {
    }

    auto GLFWWindow::Initialize(int _width, int _height, std::string _title, RenderBackend backend) -> bool {
        if (!glfwInit()) {
            std::cout << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // glfw window creation
        // --------------------
        m_window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
        if (m_window == nullptr) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(0);
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *, int w, int h) {
            glViewport(0, 0, w, h);
        });

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return false;
        }
        return true;
    }

    auto GLFWWindow::Close() -> void {
        if (m_window) {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
        glfwTerminate();
    }

    auto GLFWWindow::ShouldClose() -> bool {
        return m_window && glfwWindowShouldClose(m_window);
    }

    auto GLFWWindow::SetShouldClose(bool value) -> void {
        if (m_window) {
            glfwSetWindowShouldClose(m_window, value ? GLFW_TRUE : GLFW_FALSE);
        }
    }

    auto GLFWWindow::PollEvents() -> void {
        glfwPollEvents();
    }

    auto GLFWWindow::SwapBuffers() -> void {
        if (m_window) {
            glfwSwapBuffers(m_window);
        }
    }

    auto GLFWWindow::GetFramebufferSize(int &width, int &height) const -> void {
        if (m_window) {
            glfwGetFramebufferSize(m_window, &width, &height);
        } else {
            width = 0;
            height = 0;
        }
    }

    auto GLFWWindow::SetTitle(const char *title) -> void {
        if (m_window) {
            glfwSetWindowTitle(m_window, title);
        }
    }

    auto GLFWWindow::IsKeyPressed(Key key) const -> bool {
        if (!m_window) return false;

        int glfwKey = KeyMap(key);
        if (glfwKey == GLFW_KEY_UNKNOWN) return false;

        return glfwGetKey(m_window, glfwKey) == GLFW_PRESS;
    }

    auto GLFWWindow::GetCursorPos(double &x, double &y) const -> void {
        if (m_window) {
            glfwGetCursorPos(m_window, &x, &y);
        } else {
            x = 0.0;
            y = 0.0;
        }
    }

    void *GLFWWindow::GetNativeWindow() {
        return m_window;
    }

    auto GLFWWindow::KeyMap(Key key) -> int {
        using Key = Funccia::Graphic::Key;

        switch (key) {
            // Alphanumeric
            case Key::A: return GLFW_KEY_A;
            case Key::B: return GLFW_KEY_B;
            case Key::C: return GLFW_KEY_C;
            case Key::D: return GLFW_KEY_D;
            case Key::E: return GLFW_KEY_E;
            case Key::F: return GLFW_KEY_F;
            case Key::G: return GLFW_KEY_G;
            case Key::H: return GLFW_KEY_H;
            case Key::I: return GLFW_KEY_I;
            case Key::J: return GLFW_KEY_J;
            case Key::K: return GLFW_KEY_K;
            case Key::L: return GLFW_KEY_L;
            case Key::M: return GLFW_KEY_M;
            case Key::N: return GLFW_KEY_N;
            case Key::O: return GLFW_KEY_O;
            case Key::P: return GLFW_KEY_P;
            case Key::Q: return GLFW_KEY_Q;
            case Key::R: return GLFW_KEY_R;
            case Key::S: return GLFW_KEY_S;
            case Key::T: return GLFW_KEY_T;
            case Key::U: return GLFW_KEY_U;
            case Key::V: return GLFW_KEY_V;
            case Key::W: return GLFW_KEY_W;
            case Key::X: return GLFW_KEY_X;
            case Key::Y: return GLFW_KEY_Y;
            case Key::Z: return GLFW_KEY_Z;

            // Numbers
            case Key::Num0: return GLFW_KEY_0;
            case Key::Num1: return GLFW_KEY_1;
            case Key::Num2: return GLFW_KEY_2;
            case Key::Num3: return GLFW_KEY_3;
            case Key::Num4: return GLFW_KEY_4;
            case Key::Num5: return GLFW_KEY_5;
            case Key::Num6: return GLFW_KEY_6;
            case Key::Num7: return GLFW_KEY_7;
            case Key::Num8: return GLFW_KEY_8;
            case Key::Num9: return GLFW_KEY_9;

            // Function keys
            case Key::F1: return GLFW_KEY_F1;
            case Key::F2: return GLFW_KEY_F2;
            case Key::F3: return GLFW_KEY_F3;
            case Key::F4: return GLFW_KEY_F4;
            case Key::F5: return GLFW_KEY_F5;
            case Key::F6: return GLFW_KEY_F6;
            case Key::F7: return GLFW_KEY_F7;
            case Key::F8: return GLFW_KEY_F8;
            case Key::F9: return GLFW_KEY_F9;
            case Key::F10: return GLFW_KEY_F10;
            case Key::F11: return GLFW_KEY_F11;
            case Key::F12: return GLFW_KEY_F12;

            // Arrow keys
            case Key::Up: return GLFW_KEY_UP;
            case Key::Down: return GLFW_KEY_DOWN;
            case Key::Left: return GLFW_KEY_LEFT;
            case Key::Right: return GLFW_KEY_RIGHT;

            // Special keys
            case Key::Space: return GLFW_KEY_SPACE;
            case Key::Enter: return GLFW_KEY_ENTER;
            case Key::Escape: return GLFW_KEY_ESCAPE;
            case Key::Tab: return GLFW_KEY_TAB;
            case Key::Backspace: return GLFW_KEY_BACKSPACE;
            case Key::Delete: return GLFW_KEY_DELETE;
            case Key::Insert: return GLFW_KEY_INSERT;
            case Key::Home: return GLFW_KEY_HOME;
            case Key::End: return GLFW_KEY_END;
            case Key::PageUp: return GLFW_KEY_PAGE_UP;
            case Key::PageDown: return GLFW_KEY_PAGE_DOWN;

            // Modifiers
            case Key::LeftShift: return GLFW_KEY_LEFT_SHIFT;
            case Key::RightShift: return GLFW_KEY_RIGHT_SHIFT;
            case Key::LeftControl: return GLFW_KEY_LEFT_CONTROL;
            case Key::RightControl: return GLFW_KEY_RIGHT_CONTROL;
            case Key::LeftAlt: return GLFW_KEY_LEFT_ALT;
            case Key::RightAlt: return GLFW_KEY_RIGHT_ALT;
            case Key::LeftSuper: return GLFW_KEY_LEFT_SUPER;
            case Key::RightSuper: return GLFW_KEY_RIGHT_SUPER;

            default: return GLFW_KEY_UNKNOWN;
        }
    }
}
*/
