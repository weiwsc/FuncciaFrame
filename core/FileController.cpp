//
// Created by Wangsicong Wei on 2025-06-18.
//

#include "FileController.h"

#include "util.h"
#include <fstream>
#include <iostream>

namespace Funccia::Core {
    FileController::FileController() {
        m_handle = nullptr;
        m_readSuccess = false;
        m_thread = {};
    }
    FileController::~FileController() = default;
    auto FileController::GetCurrentDirectory() -> string {
        //char buff[FILENAME_MAX];
        //FF_ASSERT(GetCurrentDir(buff, FILENAME_MAX) != nullptr, "Could not get current directory");
        //return string(buff);
    }

    /// Find the size given the file path.
    /// int64_t is used to avoid narrow conversion with ftell(), and since int can only support file up to about 2gb
    /// @param _filePath
    /// @return size of the file
    auto FileController::GetFileSize(const string& _filePath) -> size_t {
        m_handle = fopen(_filePath.c_str(), "rb");
        FF_ASSERT(m_handle != nullptr, "Could not open file.");
        if (m_handle != nullptr) {
            FF_ASSERT(fseek(m_handle, 0, SEEK_END) == 0, "Could not seek to end of file.");
            size_t fileSize = std::ftell(m_handle);
            FF_ASSERT(fileSize != -1L, "Could not determine file size.");
            FF_ASSERT(fclose(m_handle) == 0, "Could not close file.");
            return  fileSize;
        }
        return -1;
    }

    auto FileController::GetFileSizeStream(const string& _filePath) -> std::optional<std::streamsize> {
        std::fstream file(_filePath, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << _filePath << '\n';
            return std::nullopt;
        }
        file.seekg(0, std::fstream::end);
        std::streampos pos = file.tellg();
        file.close();
        if (pos == -1) {
            std::cerr << "tellg() failed for file: " << _filePath << '\n';
            return std::nullopt;
        }
        return {static_cast<std::size_t>(pos)};
    }

    auto FileController::ReadFile(const string& _filePath, byte *_buffer, size_t _bufferSize) -> bool {
        m_readSuccess = false;
        m_handle = fopen(_filePath.c_str(), "rb");
        FF_ASSERT(m_handle != nullptr, "Could not open file.");
        if (m_handle != nullptr) {
            FF_ASSERT(fread(_buffer, 1, _bufferSize, m_handle) == _bufferSize, "All bytes not read from file.");
            FF_ASSERT(ferror(m_handle) == 0, "Error reading from file.");
            FF_ASSERT(fclose(m_handle) == 0, "Could not close file.");
            m_readSuccess = true;
        }
        if (m_thread.joinable()) {
            m_thread.detach();
        }
        return m_readSuccess;
    }

    void FileController::ReadFileAsync(const string& _filePath, byte* _buffer, size_t _bufferSize) {
        m_thread = std::thread(&FileController::ReadFile, this, _filePath, _buffer, _bufferSize);

    }


    auto FileController::ReadFileStream(const string& _filePath, byte* _buffer, std::streamsize _bufferSize) -> bool {
        std::fstream file(_filePath, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << _filePath << '\n';
            return false;
        }

        file.read(reinterpret_cast<char*>(_buffer), _bufferSize);

        bool readStatus = file.gcount() == _bufferSize;
        if (!readStatus) {
            std::cerr << "Warning: only read " << file.gcount() << " of " << _bufferSize << " bytes.\n";
        }

        file.close();
        return readStatus;
    }
} // Core
// Funccia