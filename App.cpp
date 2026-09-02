//
// Created by Wangsicong Wei on 2026-01-14.
//

#include "App.h"

#include "core/AssetController.h"
#include "core/FileWatcher.h"
#include "core/ObjectPoolManager.h"
#include "core/ProfileTimer.h"
#include "graphic/gl/GLTextRenderer.h"
#include "graphic/gl/GLUiRenderer.h"
#include "graphic/gl/SDLWindow.h"
#include "graphic/RenderBackend.h"
#include "graphic/RenderTypes.h"
#include "graphic/WindowController.h"
#include "graphic/gl/GLGraphicsDevice.h"
#include "UI/JsonUiBuilder.h"
#include "UI/Window.h"
#include "util/FpsCounter.h"

#include <iostream>

namespace vva {
    namespace {
        constexpr auto kRenderBackend = gfx::RenderBackend::OpenGL;
    }

    App::App() = default;

    App::~App() = default;

    auto App::Init() -> void {
        application_metadata = {
            .applicationTitle = "FuncciaFrame"
        };
        InitCoreSystem();
        InitWindow(kRenderBackend);
        InitGraphicsDevice();
        InitUIRenderer();
        InitUIWindow();
        InitTextRenderer();
        InitFileWatcher();
        InitFpsCounter();
    }

    auto App::Update() -> void {
        while (!window->ShouldClose()) {
            {
                PROFILE_SCOPE("Mics");
                file_watcher->Poll();
                window->PollEvents();

                if (window->IsKeyPressed(gfx::Key::Escape)) {
                    window->SetShouldClose(true);
                }

                fps_counter->Update(*window);
            }
            {
                PROFILE_SCOPE("Begin Frame");
                int winW, winH;
                window->GetFramebufferSize(winW, winH);
                graphics_device->BeginFrame({
                    .framebufferSize = {winW, winH},
                    .clearColor = {1.0f, 1.0f, 1.0f, 1.0f},
                });
                ui_renderer->BeginFrame();
                text_renderer->BeginFrame();
            }

            int winW, winH;
            {
                PROFILE_SCOPE("total ui processing and submit");
                window->GetFramebufferSize(winW, winH);
                ui_window->Render(*ui_renderer, *text_renderer, 0, 0, glm::vec2(winW, winH));
            }
            {
                PROFILE_SCOPE("UI rendering");
                ui_renderer->Render(glm::vec2(winW, winH));
            }
            {
                PROFILE_SCOPE("Text rendering");
                text_renderer->Render(glm::vec2(winW, winH));
            }
            {
                PROFILE_SCOPE("swap buffer");
                window->SwapBuffers();
            }
        }
    }

    auto App::Shutdown() -> void {
        window->Close();
        vva::core::AssetController::Instance().Clear();
    }

    auto App::InitCoreSystem() -> void {
        vva::core::AssetController::Instance().Initialize(100 * 1024 * 1024);
    }

    auto App::InitWindow(gfx::RenderBackend backend) -> void {
        auto sdlWindow = std::make_unique<vva::gfx::GL::SDLWindow>();
        if (!sdlWindow->Initialize(1920, 1080, application_metadata.applicationTitle, backend)) {
            std::cerr << "Failed to initialize window!" << std::endl;
        }

        auto& windowController = vva::gfx::WindowController::Instance();
        windowController.SetWindow(std::move(sdlWindow));
        window = windowController.GetWindow();
    }

    auto App::InitFpsCounter() -> void {
        fps_counter = std::make_unique<vva::util::FpsCounter>();
    }

    auto App::InitGraphicsDevice() -> void {
        graphics_device = std::make_unique<vva::gfx::GL::GLGraphicsDevice>();
        graphics_device->Init(*window);
    }

    auto App::InitUIRenderer() -> void {
        ui_renderer = std::make_unique<vva::gfx::GL::GLUiRenderer>();
        ui_renderer->Initialize("../graphic/shaders/vertex/UIShader-ver.glsl",
                                "../graphic/shaders/fragment/UIShader-frag.glsl");
    }

    auto App::InitUIWindow() -> void {
        ui_window = std::make_unique<vva::UI::Window>(window);


        vva::UI::JsonUiBuilder::ReadComponentSchema(
            "../graphic/assets/test.component-schema.json");
        vva::UI::JsonUiBuilder::ReadStyleSheet(
            "../graphic/assets/test.style.json");
        ui_window->ReloadFromJSON("../graphic/assets/test.funccia-ui.json");
    }

    auto App::InitTextRenderer() -> void {
        text_renderer = std::make_unique<vva::gfx::GL::GLTextRenderer>();
        text_renderer->Initialize(
            "../graphic/shaders/vertex/FontShader1-ver.glsl",
            "../graphic/shaders/fragment/FontShader1-fag.glsl");
    }

    auto App::InitFileWatcher() -> void {
        file_watcher = std::make_unique<vva::core::FileWatcher>();
        file_watcher->Add("../graphic/assets/test.funccia-ui.json",
                          [&](const std::filesystem::path& path) {
                              ui_window->ReloadFromJSON(path.string());
                          }
        );
        file_watcher->Add("../graphic/assets/test.component-schema.json",
                          [&](const std::filesystem::path& path) {
                              vva::UI::JsonUiBuilder::ReadComponentSchema(path.string());
                              ui_window->ReloadFromJSON(
                                  "../graphic/assets/test.funccia-ui.json");
                          }
        );
        file_watcher->Add("../graphic/assets/test.style.json",
                          [&](const std::filesystem::path& path) {
                              vva::UI::JsonUiBuilder::ReadStyleSheet(path.string());
                              ui_window->ReloadFromJSON("../graphic/assets/test.funccia-ui.json");
                          }
        );
        file_watcher->Start();
    }
} // Funccia
