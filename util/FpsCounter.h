//
// Created by Wangsicong Wei on 2026-01-14.
//

#pragma once
#include <chrono>

namespace Funccia::Graphic {
    class WindowInterface;
}

namespace Funccia::Util {
    class FpsCounter {
    public:
        FpsCounter();
        void Update(Graphic::WindowInterface& window);
        [[nodiscard]] auto GetFPS() const -> double { return fps; }
        [[nodiscard]] auto GetFrameMS() const -> double { return ms; }
        [[nodiscard]] auto GetDeltaTime() const -> float { return deltaTime; }
    private:
        std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> lastTime;
        std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> fpsWindowStart;
        int framesSinceUpdate = 0;
        float deltaTime = 0;
        double fps = 0;
        double ms = 0;
    };
} // Funccia::Util
