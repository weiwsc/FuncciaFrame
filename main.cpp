
#include <vector>
#include "App.h"
#include "graphic/gl/SDLWindow.h"
#include "graphic/vulkan/VulkanRenderer.h"
#include <iostream>

#include "util/Log.h"
using namespace std;


int main() {
    //Funccia::App& app = Funccia::App::Instance();
    //app.Init();
    //app.Update();
    //app.Shutdown();
    try {
        Funccia::Engine::Log::init();
        Funccia::Graphic::GL::SDLWindow window;

        if (!window.Initialize(1920, 1080, "FuncciaFrame", Funccia::Graphic::RenderBackend::Vulkan)) {
            std::cerr << "Failed to initialize Vulkan window." << std::endl;
            return 1;
        }

        Funccia::Graphic::Vulkan::VulkanRenderer renderer = Funccia::Graphic::Vulkan::VulkanRenderer::CreateVulkanRenderer(window);



        //vva_log_info("test");
        vva_log_error("error test!! Window size: {}x{}", 1920, 1080);
        vva_log_critical("test critical");
        vva_log_trace("hello trace");
        vva_log_warning("Remielle.");

        Funccia::Engine::Log::shutdown();

        return 0;
    }
    catch (exception& e) {
        vva_log_critical("fatal application error: {}", e.what());
        Funccia::Engine::Log::shutdown();
        return 1;
    }
}
