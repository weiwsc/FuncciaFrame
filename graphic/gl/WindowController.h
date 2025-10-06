//
// Created by Wangsicong Wei on 2025-09-02.
//

#ifndef FUNCCIAFRAME_WINDOWCONTROLLER_H
#define FUNCCIAFRAME_WINDOWCONTROLLER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SDLWindow.h"
#include "../WindowInterface.h"
#include "../../core/Singleton.h"

namespace Funccia::Graphic::GL {
    class WindowController : public Core::Singleton<WindowController>{
    public:
        WindowController();
        virtual ~WindowController();

        auto GetWindow() -> WindowInterface* {if (m_window == nullptr) NewWindow(); return m_window; }

        void NewWindow();


    private:
        WindowInterface *m_window;
    };
}

#endif //FUNCCIAFRAME_WINDOWCONTROLLER_H