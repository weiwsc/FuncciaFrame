//
// Created by Wangsicong Wei on 2025-10-05.
//

#include "SDLWindow.h"

#include <iostream>

#include "glad/glad.h"

namespace Funccia::Graphic::GL {
    SDLWindow::~SDLWindow() {
        delete m_mouse;
    }

    auto SDLWindow::Initialize(int _width, int _height, std::string _title, RenderBackend backend) -> bool {
        Close();
        m_backend = backend;

        // Initialize SDL only once for first window
        if (s_instanceCount == 0) {
            if (!SDL_Init(SDL_INIT_VIDEO)) {
                std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
                return false;
            }
        }
        delete m_mouse;
        m_mouse = new Input::MouseState();

        bool initialized = false;
        switch (m_backend) {
            case RenderBackend::OpenGL:
                initialized = InitializeOpenGLWindow(_width, _height, _title);
                break;
            case RenderBackend::Vulkan:
                initialized = InitializeVulkanWindow(_width, _height, _title);
                break;
        }

        if (!initialized) {
            delete m_mouse;
            m_mouse = nullptr;

            if (s_instanceCount == 0) {
                SDL_Quit();
            }
            return false;
        }

        s_instanceCount++;
        FinishInitialization();
        return true;
    }

    auto SDLWindow::InitializeOpenGLWindow(int width, int height, const std::string& title) -> bool {
        // Set OpenGL attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#ifdef __APPLE__
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

        // Double buffering
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // Anti-aliasing
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);

        // Create window
        m_window = SDL_CreateWindow(
            title.c_str(),
            width,
            height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
        );

        if (!m_window) {
            std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
            return false;
        }

        // Create OpenGL context
        m_glContext = SDL_GL_CreateContext(m_window);
        if (!m_glContext) {
            std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
            return false;
        }

        // Make context current
        SDL_GL_MakeCurrent(m_window, m_glContext);

        // Set VSync (0 = off, 1 = on, -1 = adaptive)
        SDL_GL_SetSwapInterval(0);

        // Load OpenGL functions with GLAD
        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            SDL_GL_DestroyContext(m_glContext);
            SDL_DestroyWindow(m_window);
            m_glContext = nullptr;
            m_window = nullptr;
            return false;
        }
        return true;
    }

    auto SDLWindow::InitializeVulkanWindow(int width, int height, const std::string& title) -> bool {
        (void) width;
        (void) height;
        (void) title;
        m_window = SDL_CreateWindow(
                title.data(),
                width, height,
                SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
        );
        if (!m_window) {
            std::cerr << "Error creating window." << std::endl;
            return false;
        }


        // Vulkan window creation will live here once the Vulkan backend owns surface setup.
        std::cerr << "Vulkan SDL window initialization is not implemented yet." << std::endl;
        return false;
    }

    auto SDLWindow::FinishInitialization() -> void {
        // Get initial keyboard state
        m_keyboardState = SDL_GetKeyboardState(nullptr);

        std::cout << "SDL Window initialized successfully" << std::endl;

        m_displayScale = SDL_GetWindowDisplayScale(m_window);
        std::cout << "Display scale: "<< m_displayScale << std::endl;
    }

    auto SDLWindow::Close() -> void {
        const bool hadOpenResources = m_glContext != nullptr || m_window != nullptr;

        if (m_glContext) {
            SDL_GL_DestroyContext(m_glContext);
            m_glContext = nullptr;
        }

        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }

        if (hadOpenResources && s_instanceCount > 0) {
            s_instanceCount--;
        }
        if (hadOpenResources && s_instanceCount == 0) {
            SDL_Quit();
        }
    }

    auto SDLWindow::ShouldClose() -> bool {
        return m_shouldClose;
    }

    auto SDLWindow::SetShouldClose(bool value) -> void {
        m_shouldClose = value;
    }

    auto SDLWindow::PollEvents() -> void {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    m_shouldClose = true;
                    break;

                case SDL_EVENT_MOUSE_MOTION:
                    m_mouse->SetPosition({event.motion.x, event.motion.y});
                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) m_mouse->SetButton(Input::MouseButton::Left, true);
                    if (event.button.button == SDL_BUTTON_MIDDLE) m_mouse->SetButton(Input::MouseButton::Middle, true);
                    if (event.button.button == SDL_BUTTON_RIGHT) m_mouse->SetButton(Input::MouseButton::Right, true);
                    break;

                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (event.button.button == SDL_BUTTON_LEFT) m_mouse->SetButton(Input::MouseButton::Left, false);
                    if (event.button.button == SDL_BUTTON_MIDDLE) m_mouse->SetButton(Input::MouseButton::Middle, false);
                    if (event.button.button == SDL_BUTTON_RIGHT) m_mouse->SetButton(Input::MouseButton::Right, false);
                    break;

                case SDL_EVENT_MOUSE_WHEEL: {
                    float scrollY = event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -event.wheel.y : event.wheel.y;
                    m_mouse->SetScroll({event.wheel.x, scrollY});
                    break;
                }

                case SDL_EVENT_WINDOW_RESIZED: {
                    int w = event.window.data1;
                    int h = event.window.data2;
                    if (m_backend == RenderBackend::OpenGL) {
                        glViewport(0, 0, w, h);
                    }
                }
                break;
                default:
                    break;
            }
        }
        m_keyboardState = SDL_GetKeyboardState(nullptr);
    }

    auto SDLWindow::SwapBuffers() -> void {
        if (m_window && m_backend == RenderBackend::OpenGL) {
            SDL_GL_SwapWindow(m_window);
        }
    }

    auto SDLWindow::GetFramebufferSize(int &width, int &height) const -> void {
        if (m_window) {
            SDL_GetWindowSizeInPixels(m_window, &width, &height);
        } else {
            width = 0;
            height = 0;
        }
    }

    auto SDLWindow::SetTitle(const char *title) -> void {
        if (m_window) {
            SDL_SetWindowTitle(m_window, title);
        }
    }

    auto SDLWindow::IsKeyPressed(Key key) const -> bool {
        if (!m_window || !m_keyboardState) return false;

        SDL_Scancode scancode = MapKey(key);
        if (scancode == SDL_SCANCODE_UNKNOWN) return false;

        return m_keyboardState[scancode];
    }

    auto SDLWindow::GetCursorPos(double &x, double &y) const -> void {
        if (m_window) {
            float fx, fy;

            SDL_GetMouseState(&fx, &fy);
            m_mouse->SetPosition({fx, fy});
            x = static_cast<double>(fx);
            y = static_cast<double>(fy);
        } else {
            x = 0.0;
            y = 0.0;
        }
    }

    void *SDLWindow::GetNativeWindow() {
        return m_window;
    }

    auto SDLWindow::Mouse() -> Input::MouseState * {
        return m_mouse;
    }

    auto SDLWindow::GetDisplayScale() -> float {
        return m_displayScale;
    }

    auto SDLWindow::MapKey(Key key) -> SDL_Scancode {
        using Key = Funccia::Graphic::Key;

        switch (key) {
            // Alphanumeric
            case Key::A: return SDL_SCANCODE_A;
            case Key::B: return SDL_SCANCODE_B;
            case Key::C: return SDL_SCANCODE_C;
            case Key::D: return SDL_SCANCODE_D;
            case Key::E: return SDL_SCANCODE_E;
            case Key::F: return SDL_SCANCODE_F;
            case Key::G: return SDL_SCANCODE_G;
            case Key::H: return SDL_SCANCODE_H;
            case Key::I: return SDL_SCANCODE_I;
            case Key::J: return SDL_SCANCODE_J;
            case Key::K: return SDL_SCANCODE_K;
            case Key::L: return SDL_SCANCODE_L;
            case Key::M: return SDL_SCANCODE_M;
            case Key::N: return SDL_SCANCODE_N;
            case Key::O: return SDL_SCANCODE_O;
            case Key::P: return SDL_SCANCODE_P;
            case Key::Q: return SDL_SCANCODE_Q;
            case Key::R: return SDL_SCANCODE_R;
            case Key::S: return SDL_SCANCODE_S;
            case Key::T: return SDL_SCANCODE_T;
            case Key::U: return SDL_SCANCODE_U;
            case Key::V: return SDL_SCANCODE_V;
            case Key::W: return SDL_SCANCODE_W;
            case Key::X: return SDL_SCANCODE_X;
            case Key::Y: return SDL_SCANCODE_Y;
            case Key::Z: return SDL_SCANCODE_Z;

            // Numbers
            case Key::Num0: return SDL_SCANCODE_0;
            case Key::Num1: return SDL_SCANCODE_1;
            case Key::Num2: return SDL_SCANCODE_2;
            case Key::Num3: return SDL_SCANCODE_3;
            case Key::Num4: return SDL_SCANCODE_4;
            case Key::Num5: return SDL_SCANCODE_5;
            case Key::Num6: return SDL_SCANCODE_6;
            case Key::Num7: return SDL_SCANCODE_7;
            case Key::Num8: return SDL_SCANCODE_8;
            case Key::Num9: return SDL_SCANCODE_9;

            // Function keys
            case Key::F1: return SDL_SCANCODE_F1;
            case Key::F2: return SDL_SCANCODE_F2;
            // ... etc

            // Arrow keys
            case Key::Up: return SDL_SCANCODE_UP;
            case Key::Down: return SDL_SCANCODE_DOWN;
            case Key::Left: return SDL_SCANCODE_LEFT;
            case Key::Right: return SDL_SCANCODE_RIGHT;

            // Special keys
            case Key::Space: return SDL_SCANCODE_SPACE;
            case Key::Enter: return SDL_SCANCODE_RETURN;
            case Key::Escape: return SDL_SCANCODE_ESCAPE;
            case Key::Tab: return SDL_SCANCODE_TAB;
            case Key::Backspace: return SDL_SCANCODE_BACKSPACE;

            // Modifiers
            case Key::LeftShift: return SDL_SCANCODE_LSHIFT;
            case Key::RightShift: return SDL_SCANCODE_RSHIFT;
            case Key::LeftControl: return SDL_SCANCODE_LCTRL;
            case Key::RightControl: return SDL_SCANCODE_RCTRL;

            default: return SDL_SCANCODE_UNKNOWN;
        }
    }
}
