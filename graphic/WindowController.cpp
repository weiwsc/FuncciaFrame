//
// Created by Wangsicong Wei on 2025-09-02.
//

#include "WindowController.h"

#include <utility>

namespace vva::gfx {
    WindowController::WindowController() {
        m_window = nullptr;
    }

    WindowController::~WindowController() {
        if (m_window != nullptr) {
            m_window->Close();
        }
    }

    auto WindowController::SetWindow(std::unique_ptr<WindowInterface> window) -> void {
        if (m_window != nullptr) {
            m_window->Close();
        }
        m_window = std::move(window);
    }
}
