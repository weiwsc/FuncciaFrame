//
// Created by Wangsicong Wei on 2025-10-05.
//

#ifndef FUNCCIAFRAME_SDLWINDOW_H
#define FUNCCIAFRAME_SDLWINDOW_H
#include <SDL3/SDL.h>
#include "../WindowInterface.h"
#include "../../input/MouseState.h"

namespace vva::gfx::GL {
    class SDLWindow : public WindowInterface {
    public:
        SDLWindow() = default;

        ~SDLWindow() override;

        auto Initialize(int _width = 1920, int _height = 1080, std::string _title = "FuncciaFrame",
                        RenderBackend backend = RenderBackend::OpenGL) -> bool override;

        auto Close() -> void override;

        auto ShouldClose() -> bool override;

        auto SetShouldClose(bool value) -> void override;

        auto PollEvents() -> void override;

        auto SwapBuffers() -> void override;

        auto GetFramebufferSize(int &width, int &height) const -> void override;

        auto SetTitle(const char *title) -> void override;

        [[nodiscard]] auto IsKeyPressed(Key key) const -> bool override;

        auto GetCursorPos(double &x, double &y) const -> void override;

        void *GetNativeWindow() override;

        [[nodiscard]] auto Mouse() -> Input::MouseState * override;

        auto GetDisplayScale()->float override;

    private:
        auto InitializeOpenGLWindow(int width, int height, const std::string& title) -> bool;
        auto InitializeVulkanWindow(int width, int height, const std::string& title) -> bool;
        auto FinishInitialization() -> void;

        SDL_Window *m_window{nullptr};
        const bool *m_keyboardState{nullptr};
        SDL_GLContext m_glContext{nullptr};
        bool m_shouldClose{false};
        RenderBackend m_backend{RenderBackend::OpenGL};
        Input::MouseState *m_mouse{nullptr};

        static auto MapKey(Key key) -> SDL_Scancode;

        static inline int s_instanceCount = 0;
        float m_displayScale{1.0f};
    };
}
#endif //FUNCCIAFRAME_SDLWINDOW_H
