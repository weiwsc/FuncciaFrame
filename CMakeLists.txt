cmake_minimum_required(VERSION 3.31)
project(FuncciaFrame)

set(CMAKE_CXX_STANDARD 23)

add_executable(FuncciaFrame main.cpp
        core/FileController.cpp
        core/FileController.h
        core/Singleton.h
        core/util.cpp
        core/util.h)
