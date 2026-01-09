// FileWatcher.h
#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <atomic>
#include <filesystem>
#include <functional>
#include <thread>
#include <chrono>

namespace Funccia {

class FileWatcher {
public:
    using Callback = std::function<void(const std::filesystem::path&)>;

    FileWatcher(std::filesystem::path path, Callback callback,
                std::chrono::milliseconds interval = std::chrono::milliseconds(500))
        : m_path(std::move(path))
        , m_callback(std::move(callback))
        , m_interval(interval)
        , m_running(false)
    {}

    ~FileWatcher() {
        Stop();
    }

    // Non-copyable
    FileWatcher(const FileWatcher&) = delete;
    FileWatcher& operator=(const FileWatcher&) = delete;

    void Start() {
        if (m_running) return;
        m_running = true;
        m_lastWrite = std::filesystem::last_write_time(m_path);

        m_thread = std::thread([this]() {
            while (m_running) {
                std::this_thread::sleep_for(m_interval);
                if (!m_running) break;

                try {
                    auto current = std::filesystem::last_write_time(m_path);
                    if (current != m_lastWrite) {
                        m_lastWrite = current;
                        m_changed = true;
                    }
                } catch (const std::filesystem::filesystem_error&) {
                    // File might be mid-write, ignore
                }
            }
        });
    }

    void Stop() {
        m_running = false;
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    // Call this from main thread each frame
    void Poll() {
        if (m_changed.exchange(false)) {
            m_callback(m_path);
        }
    }

private:
    std::filesystem::path m_path;
    Callback m_callback;
    std::chrono::milliseconds m_interval;

    std::atomic<bool> m_running;
    std::atomic<bool> m_changed{false};
    std::filesystem::file_time_type m_lastWrite;
    std::thread m_thread;
};

} // namespace Funccia

#endif