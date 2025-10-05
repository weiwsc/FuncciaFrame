//
// Created by Wangsicong Wei on 2025-10-05.
//

#ifndef FUNCCIAFRAME_GLFWWINDOW_H
#define FUNCCIAFRAME_GLFWWINDOW_H
#include "../WindowInterface.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace Funccia::Graphic::GL {
    class GLFWWindow : public WindowInterface {
    public:
        GLFWWindow() = default;
        ~GLFWWindow() override;

        auto Initialize(int _width = 1920, int _height = 1080, std::string _title = "FuncciaFrame") -> bool override;

        auto Close() -> void override;

        auto ShouldClose() -> bool override;

        auto SetShouldClose(bool value) -> void override;

        auto PollEvents() -> void override;

        auto SwapBuffers() -> void override;

        auto GetFramebufferSize(int &width, int &height) const -> void override;

        auto SetTitle(const char *title) -> void override;

        [[nodiscard]] auto IsKeyPressed(Key key) const -> bool override;

        auto GetCursorPos(double &x, double &y) const -> void override;

        void * GetNativeWindow() override;

    private:
        static auto KeyMap(Key key) -> int;
        GLFWwindow* m_window {nullptr};
    };
}

#endif //FUNCCIAFRAME_GLFWWINDOW_H