//
// Created by Dvillera Vian on 2026-01-14.
//

#include "GLGraphicsDevice.h"

#include "glad/glad.h"

namespace vva::gfx::GL
{
    void GLGraphicsDevice::Init(WindowInterface& window)
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

    void GLGraphicsDevice::BeginFrame(const FrameContext& frameContext) {
        glClearColor(frameContext.clearColor.r, frameContext.clearColor.g,
                     frameContext.clearColor.b, frameContext.clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, frameContext.framebufferSize.x, frameContext.framebufferSize.y);
    }
}
