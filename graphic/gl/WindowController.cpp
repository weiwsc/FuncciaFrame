//
// Created by Wangsicong Wei on 2025-09-02.
//

#include "WindowController.h"

#include <iostream>

#include "SDLWindow.h"
#include "../../core/util.h"

namespace Funccia::Graphic::GL{
    WindowController::WindowController() {
        m_window = nullptr;
    }

    WindowController::~WindowController() {
        if (m_window != nullptr) {
            m_window->Close();
            m_window = nullptr;
        }
    }

    void WindowController::NewWindow() {
        m_window = reinterpret_cast<WindowInterface *>(new SDLWindow());
        m_window->Initialize(1920, 1080, "Funccia");
    }
}
