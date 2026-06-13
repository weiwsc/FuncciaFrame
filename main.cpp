
#include <vector>
#include "App.h"
#include "graphic/gl/SDLWindow.h"
#include "graphic/vulkan/VulkanApplication.h"
#include <iostream>
using namespace std;


int main() {
    //Funccia::App& app = Funccia::App::Instance();
    //app.Init();
    //app.Update();
    //app.Shutdown();
    Funccia::Graphic::GL::SDLWindow window;

    if (!window.Initialize(1920, 1080, "FuncciaFrame", Funccia::Graphic::RenderBackend::Vulkan)) {
        std::cerr << "Failed to initialize Vulkan window." << std::endl;
        return 1;
    }

    Funccia::Graphic::Vulkan::VulkanApplication vkApp;
    vkApp.Init(window);
    return 0;
}
