#include <iostream>
#include <chrono>
#include "core/FileController.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto* fr = new Funccia::Core::FileController();
    const std::string filename = "/Users/dvillera/Projects/cpp/FuncciaFrameWin/FuncciaFramWIn/Controller.cpp";
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "old version " << fr->GetFileSize(filename)<< " MB" << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();

    std::cout << "using fstream " << (fr->GetFileSizeStream(filename)).value_or(0) << " MB" << std::endl;
    stop = std::chrono::high_resolution_clock::now();
    duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << std::endl;

    std::streamsize size = fr->GetFileSizeStream(filename).value_or(0);
    auto* buffer = new std::byte[size];
    fr->ReadFileStream(filename, buffer, size);
    std::cout << std::string(reinterpret_cast<char*>(buffer)) << std::endl;

    return 0;
}
