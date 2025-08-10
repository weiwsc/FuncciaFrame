//
// Created by Wangsicong Wei on 2025-06-18.
//

#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H
#include <optional>
#include <thread>
#include "Singleton.h"


namespace Funccia::Core {
    using std::string, std::byte;

    class FileController : public Singleton<FileController> {
    public:
        FileController();
        virtual ~FileController();

        [[nodiscard]]
        auto isFileReadDone() const -> bool { return !m_thread.joinable(); }

        [[nodiscard]]
        auto isFileReadSuccess() const -> bool { return m_readSuccess; }

        static
        auto GetCurrentDirectory() -> string;

        auto GetFileSize(const string &_filePath) -> size_t;

        [[nodiscard]] static
        auto GetFileSizeStream(const string& _filePath) -> std::optional<std::streamsize>;

        auto ReadFile(const string& _filePath, byte *_buffer, size_t _bufferSize) -> bool;

        auto ReadFileAsync(const string& _filePath, byte *_buffer, size_t _bufferSize) -> void;

        /// load a file into memory
        /// @param _filePath full path to the file
        /// @param _buffer byte buffer for loading the data into memory
        /// @param _bufferSize size of the file to be read
        /// @return the read status as bool
        static
        auto ReadFileStream(const string& _filePath, byte *_buffer, std::streamsize _bufferSize) -> bool;

    private:
        FILE* m_handle;
        bool m_readSuccess;
        std::thread m_thread;
    };

}

#endif //FILECONTROLLER_H
