#include <exception>
#include <filesystem>

#include "core/Log.h"
#include "gfx/vulkan/VulkanRenderer.h"
#include "platform/SDLWindow.h"

int main() {
    try {
        vva::engine::log::init();

        vva::gfx::GL::SDLWindow window;
        if (!window.Initialize(1920, 1080, "FuncciaFrame", vva::gfx::RenderBackend::Vulkan)) {
            vva_log_critical("failed to initialize Vulkan window");
            vva::engine::log::shutdown();
            return 1;
        }

        auto renderer = vva::gfx::vulkan::VulkanRenderer::createVulkanRenderer(window, {
            .app_name = "FuncciaFrame",
            .shader_dir = std::filesystem::path{VVA_ASSET_DIR} / "shaders",
        });

        vva_log_info("sandbox: renderer created, exiting");
        vva_log_info("vivian is going home!");
        vva::engine::log::shutdown();
        return 0;
    }
    catch (const std::exception& e) {
        vva_log_critical("fatal application error: {}", e.what());
        vva::engine::log::shutdown();
        return 1;
    }
}
