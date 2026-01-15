//
// Created by Dvillera Vian on 2026-01-14.
//

#include "GLGraphicsDevice.h"

#include "../WindowController.h"
#include "glad/glad.h"

namespace Funccia::Graphic::GL
{
    void GLGraphicsDevice::Init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
    }

    void GLGraphicsDevice::Update() {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int winW, winH;
        WindowController::Instance().GetWindow()->GetFramebufferSize(winW, winH);
        glViewport(0, 0, winW, winH);
    }
}
