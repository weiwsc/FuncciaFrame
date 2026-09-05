#pragma once

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>

namespace vva::profile {
    enum class Section : std::size_t {
        Frame,
        Events,
        FenceWait,
        Acquire,
        Update,
        Record,
        Submit,
        Present,
        Count
    };

    struct Stats {
        std::array<double, static_cast<std::size_t>(Section::Count)> total_ms{};
        std::uint64_t frames{};

        // Repeated scopes accumulate, so this is per frame, not per call.
        [[nodiscard]] double msPerFrame(Section section) const noexcept {
            return frames == 0 ? 0.0
                : total_ms[static_cast<std::size_t>(section)] / static_cast<double>(frames);
        }
    };

    // Render-thread only: this collector is deliberately not synchronized.
    // Take snapshots between frames, after all scopes have finished.
    class Profiler {
    public:
        void add(Section section, double ms) noexcept {
            stats_.total_ms[static_cast<std::size_t>(section)] += ms;
        }

        void endFrame() noexcept { ++stats_.frames; }

        [[nodiscard]] Stats takeStats() noexcept {
            const auto result = stats_;
            stats_ = {};
            return result;
        }

    private:
        Stats stats_{};
    };

    inline Profiler cpu;

    // CPU wall time, including blocking; this does not measure GPU execution.
    // Nested scopes are inclusive: Frame already contains the other sections.
    class Scope {
        using Clock = std::chrono::steady_clock;

    public:
        explicit Scope(Section section) noexcept
            : section_(section), start_(Clock::now()) {}

        ~Scope() noexcept { stop(); }

        Scope(const Scope&) = delete;
        Scope& operator=(const Scope&) = delete;

        // Useful when command recording ends before the enclosing function does.
        void stop() noexcept {
            if (!running_) return;
            const auto end = Clock::now();
            running_ = false;
            cpu.add(section_, std::chrono::duration<double, std::milli>(end - start_).count());
        }

    private:
        Section section_;
        Clock::time_point start_;
        bool running_{true};
    };
}
