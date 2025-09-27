//
// Created by Wangsicong Wei on 2025-09-02.
//

#ifndef FUNCCIAFRAME_TESTRUN_H
#define FUNCCIAFRAME_TESTRUN_H
#include <cassert>
#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../core/Asset.h"
#include "../core/AssetController.h"
#include "../core/FileController.h"
#include "../core/ObjectPoolManager.h"
#include "../core/Resource.h"
#include "../test/ContainerA.h"

class TestRun {
    public:
    static void GLFWTest();
    static void OpenGLTest();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void SerializerTest();
};



#endif //FUNCCIAFRAME_TESTRUN_H
