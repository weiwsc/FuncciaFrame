//
// Created by Wangsicong Wei on 2025-09-02.
//

#include "WindowController.h"

#include <iostream>

#include "gl/SDLWindow.h"
#include "../core/util.h"

namespace Funccia::Graphic::GL {
    WindowController::WindowController() {
        m_window = nullptr;
    }

    WindowController::~WindowController() {
        if (m_window != nullptr) {
            m_window->Close();
        }
    }

    void WindowController::NewWindow() {
        m_window = std::make_unique<SDLWindow>();
        if (!m_window->Initialize(1920, 1080, "FuncciaFrame")) {
        std::cerr << "Failed to initialize window!" << std::endl;
    }
    }
}