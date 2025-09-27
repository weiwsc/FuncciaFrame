//
// Created by Wangsicong Wei on 2025-09-02.
//

#include "TestRun.h"

#include "../graphic/gl/Mesh.h"
#include "../graphic/gl/WindowController.h"


void TestRun::GLFWTest() {

}

void TestRun::OpenGLTest() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void TestRun::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void TestRun::SerializerTest() {
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
}
