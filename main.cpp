
#include <vector>
#include "App.h"
#include "graphic/gl/SDLWindow.h"
#include "graphic/vulkan/VulkanRenderer.h"
#include <iostream>

#include "util/Log.h"
using namespace std;


int main() {
    //vva::App& app = vva::App::Instance();
    //app.Init();
    //app.Update();
    //app.Shutdown();
    try {
        vva::engine::log::init();
        vva::gfx::GL::SDLWindow window;

        if (!window.Initialize(1920, 1080, "FuncciaFrame", vva::gfx::RenderBackend::Vulkan)) {
            std::cerr << "Failed to initialize Vulkan window." << std::endl;
            return 1;
        }

        vva::gfx::vulkan::VulkanRenderer renderer = vva::gfx::vulkan::VulkanRenderer::createVulkanRenderer(window);



        //vva_log_info("test");
        vva_log_error("error test!! Window size: {}x{}", 1920, 1080);
        vva_log_critical("test critical");
        vva_log_trace("hello trace");
        vva_log_warning("Remielle.");
        vva::engine::log::shutdown();

        return 0;
    }
    catch (exception& e) {
        vva_log_critical("fatal application error: {}", e.what());
        vva::engine::log::shutdown();
        return 1;
    }
}
