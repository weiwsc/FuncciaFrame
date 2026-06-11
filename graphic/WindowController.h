//
// Created by Wangsicong Wei on 2025-09-02.
//

#ifndef FUNCCIAFRAME_WINDOWCONTROLLER_H
#define FUNCCIAFRAME_WINDOWCONTROLLER_H
#include <memory>

#include "WindowInterface.h"
#include "../core/Singleton.h"

namespace Funccia::Graphic {
    class WindowController : public Core::Singleton<WindowController> {
    public:
        WindowController();

        virtual ~WindowController();

        auto GetWindow() -> WindowInterface * {
            return m_window.get();
        }

        auto SetWindow(std::unique_ptr<WindowInterface> window) -> void;

    private:
        std::unique_ptr<WindowInterface> m_window;

    };
}

#endif //FUNCCIAFRAME_WINDOWCONTROLLER_H
