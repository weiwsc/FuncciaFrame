
#include <vector>
#include "App.h"
#include "graphic/gl/SDLWindow.h"
#include "graphic/vulkan/VulkanGraphicsDevice.h"
#include <iostream>

#include "util/Log.h"
using namespace std;


int main() {
    //Funccia::App& app = Funccia::App::Instance();
    //app.Init();
    //app.Update();
    //app.Shutdown();
    Funccia::Engine::Log::init();
    Funccia::Graphic::GL::SDLWindow window;

    if (!window.Initialize(1920, 1080, "FuncciaFrame", Funccia::Graphic::RenderBackend::Vulkan)) {
        std::cerr << "Failed to initialize Vulkan window." << std::endl;
        return 1;
    }

    Funccia::Graphic::Vulkan::VulkanGraphicsDevice vkApp;
    vkApp.Init(window);

    //vva_log_info("test");
    //vva_log_debug("Window size: {}x{}", 1920, 1080);

    Funccia::Engine::Log::shutdown();
    return 0;
}
