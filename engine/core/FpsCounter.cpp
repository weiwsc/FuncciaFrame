//
// Created by Wangsicong Wei on 2026-01-14.
//

#include "FpsCounter.h"

#include "../engine/platform/WindowInterface.h"

namespace vva::util {
    FpsCounter::FpsCounter() {
        last_time_ = std::chrono::steady_clock::now();
        fps_window_start_ = last_time_;
    }

    void FpsCounter::update(gfx::WindowInterface& window) {
        const auto now = std::chrono::steady_clock::now();
        delta_time_ = std::chrono::duration<float>(now - last_time_).count();

        frames_since_update_++;
        if (const double elapsed = std::chrono::duration<double>(now - fps_window_start_).count(); elapsed >= 0.5) {
            char title_buf[128];
            fps_ = frames_since_update_ / elapsed;
            ms_ = 1000.0 / (fps_ > 0.0 ? fps_ : 1.0);
            std::snprintf(title_buf, sizeof(title_buf), "FuncciaFrame | %.1f FPS (%.2f ms)", fps_, ms_);
            window.SetTitle(title_buf);
            fps_window_start_ = now;
            frames_since_update_ = 0;
        }
        last_time_ = now;
    }
}
