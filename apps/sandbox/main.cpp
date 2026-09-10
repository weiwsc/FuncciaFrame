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
        glm::ivec2 dimension = {1920, 1080};
        if (!window.Initialize(dimension.x / 2, dimension.y / 2, "FuncciaFrame", vva::gfx::RenderBackend::Vulkan)) {
            vva_log_critical("failed to initialize Vulkan window");
            vva::engine::log::shutdown();
            return 1;
        }
        vva::util::FpsCounter fps_counter;
        auto renderer = vva::gfx::vulkan::VulkanRenderer::createVulkanRenderer(window, {
                                                                                   .app_name = "FuncciaFrame",
                                                                                   .shader_dir = std::filesystem::path{
                                                                                       VVA_ASSET_DIR
                                                                                   } / "shaders",
                                                                               });

        vva::gfx::Transform transform{};
        //transform.rotate(vva::gfx::Vector3::RIGHT,-140.f);
        transform.rotate(vva::gfx::Vector3::UP, 90.f);


        auto model = vva::gfx::vulkan::Model::create(
            transform, vva::gfx::vulkan::ModelLoadInfo{
                .mesh_path = (std::filesystem::path{VVA_ASSET_DIR}
                    / "models/viking_room.obj").string(),
                .texture_path = (std::filesystem::path{VVA_ASSET_DIR}
                    / "textures/viking_room.png").string()
            }, renderer.resourceRegistry());

        vva::gfx::vulkan::Camera camera{
            .transform = {},
            .projection = glm::perspective(glm::radians(45.0f),
                                           (float)dimension.x / (float)dimension.y,
                                           0.1f,
                                           100.0f)
        };
        // glm::perspective produces OpenGL clip space (+Y up); Vulkan NDC has +Y down.
        // Flipping Y here keeps the image upright and preserves the mesh winding, so
        // eCounterClockwise front faces + back-face culling in the pipeline stay correct.
        camera.projection[1][1] *= -1.0f;
        camera.transform.setPosition({0, 20, 5});
        camera.transform.lookAt({0, 0, 0});

        while (!window.ShouldClose()) {
            window.PollEvents();
            if (window.IsKeyPressed(vva::gfx::Key::Escape)) window.SetShouldClose(true);

            fps_counter.update(window);

            renderer.startFrame();
            camera.transform.translate({0, 0, sin(fps_counter.getFrameMs()) * fps_counter.getDeltaTime() * 10});
            double x, y{};
            window.GetCursorPos(x, y);
            renderer.updateFrameData(camera, {x, y});
            model.transform.rotate({vva::gfx::Vector3::UP}, 90.f * fps_counter.getDeltaTime());
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
