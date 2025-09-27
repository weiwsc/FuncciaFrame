//
// Created by Wangsicong Wei on 2025-09-02.
//

#include "WindowController.h"

#include <iostream>

#include "../../core/util.h"

namespace Funccia::Graphic::GL{
    WindowController::WindowController() {
        m_window = nullptr;
    }

    WindowController::~WindowController() {
        if (m_window != nullptr) {
            glfwTerminate();
            m_window = nullptr;
        }
    }

    void WindowController::NewWindow() {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // glfw window creation
        // --------------------
        m_window = glfwCreateWindow(1920, 1080, "LearnOpenGL", NULL, NULL);
        if (m_window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(0);
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow*, int w, int h){
            glViewport(0, 0, w, h);
        });

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }
    }
}
