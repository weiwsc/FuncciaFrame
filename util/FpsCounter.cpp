//
// Created by Wangsicong Wei on 2026-01-14.
//

#include "FpsCounter.h"

#include "../graphic/WindowInterface.h"

namespace Funccia::Util {
    FpsCounter::FpsCounter() {
        lastTime = std::chrono::steady_clock::now();
        fpsWindowStart = lastTime;
    }

    void FpsCounter::Update(Graphic::WindowInterface& window) {
        const auto now = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration<float>(now - lastTime).count();

        framesSinceUpdate++;
        if (const double elapsed = std::chrono::duration<double>(now - fpsWindowStart).count(); elapsed >= 0.5) {
            char titleBuf[128];
            fps = framesSinceUpdate / elapsed;
            ms = 1000.0 / (fps > 0.0 ? fps : 1.0);
            std::snprintf(titleBuf, sizeof(titleBuf), "FuncciaFrame | %.1f FPS (%.2f ms)", fps, ms);
            window.SetTitle(titleBuf);
            fpsWindowStart = now;
            framesSinceUpdate = 0;
        }
        lastTime = now;
    }
}
