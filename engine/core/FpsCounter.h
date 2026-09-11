//
// Created by Wangsicong Wei on 2026-01-14.
//

#pragma once
#include <chrono>

namespace vva::gfx {
    class WindowInterface;
}

namespace vva::util {
    class FpsCounter {
    public:
        FpsCounter();
        void update(gfx::WindowInterface& window);
        [[nodiscard]] auto getFps() const -> double { return fps_; }
        [[nodiscard]] auto getFrameMs() const -> double { return ms_; }
        [[nodiscard]] auto getDeltaTime() const -> float { return delta_time_; }
        [[nodiscard]] auto getTime() const -> float { return time_; }
    private:
        std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> last_time_;
        std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> fps_window_start_;
        int frames_since_update_ = 0;
        float delta_time_ = 0;
        double fps_ = 0;
        double ms_ = 0;
        std::chrono::steady_clock::time_point start_time_;
        float time_ = 0;
    };
} // vva::util
