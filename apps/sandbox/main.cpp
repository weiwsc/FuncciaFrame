#include <exception>
#include <filesystem>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "core/FpsCounter.h"
#include "core/Log.h"
#include "gfx/vulkan/VulkanRenderer.h"
#include "gfx/vulkan/types/Sampler.h"
#include "platform/SDLWindow.h"

int main() {
    try {
        vva::engine::log::init();

        vva::gfx::GL::SDLWindow window;
        glm::ivec2 dimension = {1920, 1080};
        if (!window.Initialize(dimension.x, dimension.y, "FuncciaFrame", vva::gfx::RenderBackend::Vulkan)) {
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

        std::vector<vva::gfx::vulkan::Model> models;
        for (int i = 0; i < 900; ++i) {
            vva::gfx::Transform transform = model.transform;
            transform.setPosition({(i % 30) * 3 - 10, (i / 30) * 3 - 10, 0});

            models.push_back(
                vva::gfx::vulkan::Model{
                    .transform = transform,
                    .mesh_handle = model.mesh_handle,
                    .texture_handle = model.texture_handle
                });
        }
        auto& texture = model.texture_handle.getTexture();
        std::array textures {
            vva::gfx::shader::param::TextureDrawData {
                .texture_index = texture.slot,
                .sampler_index = vva::gfx::vulkan::SAMPLER_LINEAR_CLAMP,
                .size = {texture.extent.width, texture.extent.height}
            }
        };

        while (!window.ShouldClose()) {
            window.PollEvents();
            if (window.IsKeyPressed(vva::gfx::Key::Escape)) window.SetShouldClose(true);

            fps_counter.update(window);

            auto frame = renderer.startFrame({
                .delta_time = fps_counter.getDeltaTime(),
                .time = fps_counter.getTime()
            });
            //camera.transform.translate({0, 0, sin(fps_counter.getFrameMs()) * fps_counter.getDeltaTime() * 10});
            camera.transform.rotate(vva::gfx::Vector3::UP, 20 * fps_counter.getDeltaTime());

            for (auto& obj : models) {
                obj.transform.rotate(vva::gfx::Vector3::UP, 20 * fps_counter.getDeltaTime());
            }

            double x, y{};
            window.GetCursorPos(x, y);
            renderer.updateSceneData(camera, {x, y}, frame);
            model.transform.rotate({vva::gfx::Vector3::UP}, 90.f * fps_counter.getDeltaTime());


            auto pass = renderer.get3dRenderPass(frame);
            pass.begin();
            renderer.drawModel(models, frame);
            pass.end();
            //
            auto pass2 = renderer.get2dRenderPass(frame);
            pass2.begin();
            ///TODO: figure out how to deal with the mouse position mismatch when the buffer size is different from requested window size
            textures[0].position = {x * 2, y * 2};
            renderer.drawTexture(textures, frame);
            pass2.end();



            renderer.endFrame(frame);
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
