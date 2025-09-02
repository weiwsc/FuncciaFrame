
#include <cassert>
#include <iostream>
#include <chrono>
#include "core/Asset.h"
#include "core/AssetController.h"
#include "core/FileController.h"
#include "core/ObjectPoolManager.h"
#include "core/Resource.h"
#include "test/ContainerA.h"

int main() {
    /*
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
    */

    Funccia::Core::AssetController::Instance().Initialize(100 * 1024 * 1024);
    Funccia::Core::ObjectPoolManager::Object1Pool = new Funccia::Core::ObjectPool<Funccia::Core::Object1>();
    Funccia::Core::ObjectPoolManager::Object2Pool = new Funccia::Core::ObjectPool<Funccia::Core::Object2>();
    Funccia::Core::ObjectPoolManager::ResourcePool = new Funccia::Core::ObjectPool<Funccia::Core::Resource>();
    Funccia::Core::ObjectPoolManager::ContainerAPool = new Funccia::Core::ObjectPool<Funccia::Core::ContainerA>();

    auto level = new Funccia::Core::ContainerA();
    level->AddResource(dynamic_cast<Funccia::Core::Resource*>(Funccia::Core::ObjectPoolManager::Object1Pool->GetResource()));
    level->Save();
    delete level;

    level = new Funccia::Core::ContainerA();
    std::cout << "Hello, World!" << std::endl;
    level->Load();





    return 0;
}
