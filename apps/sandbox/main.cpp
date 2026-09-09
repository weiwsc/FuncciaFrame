#include <exception>
#include <filesystem>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "core/FpsCounter.h"
#include "core/Log.h"
#include "gfx/vulkan/VulkanRenderer.h"
#include "platform/SDLWindow.h"

int main() {
    try {
        vva::engine::log::init();

        vva::gfx::GL::SDLWindow window;
        if (!window.Initialize(1920 / 1, 1080 / 1, "FuncciaFrame", vva::gfx::RenderBackend::Vulkan)) {
            vva_log_critical("failed to initialize Vulkan window");
            vva::engine::log::shutdown();
            return 1;
        }
        vva::util::FpsCounter fps_counter;
        auto renderer = vva::gfx::vulkan::VulkanRenderer::createVulkanRenderer(window, {
            .app_name = "FuncciaFrame",
            .shader_dir = std::filesystem::path{VVA_ASSET_DIR} / "shaders",
        });

        vva::gfx::Transform transform {};
        //transform.rotate(vva::gfx::Vector3::RIGHT,-140.f);
        transform.rotate(vva::gfx::Vector3::UP,90.f);
        

        auto model = renderer.loadModel(vva::gfx::vulkan::ModelLoadInfo{
            .transform = transform,
            .mesh_path = (std::filesystem::path{VVA_ASSET_DIR}
                         / "models/viking_room.obj").string(),
            .texture_path = (std::filesystem::path{VVA_ASSET_DIR}
                            / "textures/viking_room.png").string()
        });

        while (!window.ShouldClose()) {
            window.PollEvents();
            if (window.IsKeyPressed(vva::gfx::Key::Escape)) window.SetShouldClose(true);

            fps_counter.update(window);

            renderer.startFrame();
            renderer.getModelTransform(model).rotate({vva::gfx::Vector3::UP}, 90.f * fps_counter.getDeltaTime() );
            renderer.drawModel(model);
            renderer.endFrame();
        }
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
