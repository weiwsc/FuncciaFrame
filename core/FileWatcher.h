// FileWatcher.h
#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <atomic>
#include <filesystem>
#include <functional>
#include <thread>
#include <chrono>

namespace Funccia {
    using Callback = std::function<void(const std::filesystem::path&)>;
struct FileWatch {
    std::filesystem::path path;
    std::filesystem::file_time_type m_lastWrite;
    std::atomic<bool> m_changed{false};
    Callback callback;
};
    namespace Core {
        class FileWatcher {
        public:

            FileWatcher(
                        std::chrono::milliseconds interval = std::chrono::milliseconds(500))
                : m_interval(interval)
                , m_running(false)
            {}

            ~FileWatcher() {
                Stop();
            }

            // Non-copyable
            FileWatcher(const FileWatcher&) = delete;
            FileWatcher& operator=(const FileWatcher&) = delete;

            void Add(std::filesystem::path path, Callback callback) {
                auto watch = std::make_unique<FileWatch>();
                watch->path = std::move(path);
                watch->m_lastWrite = std::filesystem::last_write_time(watch->path);
                watch->m_changed = false;
                watch->callback = std::move(callback);
                fileWatch.push_back(std::move(watch));
            }

            void Start() {
                if (m_running) return;
                m_running = true;
                for (auto& file : fileWatch) {
                    file->m_lastWrite = std::filesystem::last_write_time(file->path);
                }
                m_thread = std::thread([this]() {
                    while (m_running) {
                        std::this_thread::sleep_for(m_interval);
                        if (!m_running) break;

                        for (auto& file : fileWatch) {
                            try {
                                auto current = std::filesystem::last_write_time(file->path);
                                if (current != file->m_lastWrite) {
                                    file->m_lastWrite = current;
                                    file->m_changed = true;
                                }
                            } catch (const std::filesystem::filesystem_error&) {
                                // File might be mid-write, ignore
                            }
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
                for (auto& file : fileWatch) {
                    if (file->m_changed.exchange(false)) {
                        file->callback(file->path);
                    }
                }
            }

        private:

            std::vector<std::unique_ptr<FileWatch>> fileWatch;

            std::chrono::milliseconds m_interval;

            std::atomic<bool> m_running;
            std::thread m_thread;
        };
    }
} // namespace Funccia

#endif