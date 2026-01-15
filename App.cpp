//
// Created by Wangsicong Wei on 2026-01-14.
//

#include "App.h"

#include "core/AssetController.h"
#include "core/FileWatcher.h"
#include "core/ObjectPoolManager.h"
#include "graphic/gl/TextRenderer.h"
#include "graphic/gl/UIRender.h"
#include "graphic/WindowController.h"
#include "graphic/gl/GLGraphicsDevice.h"
#include "UI/JsonUiBuilder.h"
#include "UI/Window.h"
#include "util/FpsCounter.h"

namespace Funccia {
    App::App() {
    }

    App::~App() {
    }

    auto App::Init() -> void {
        InitCoreSystem();
        window = Funccia::Graphic::GL::WindowController::Instance().GetWindow();
        InitGraphicsDevice();
        InitUIRenderer();
        InitUIWindow();
        InitTextRenderer();
        InitFileWatcher();
        InitFpsCounter();
    }

    auto App::Update() -> void {
        while (!window->ShouldClose()) {
            file_watcher->Poll();
            window->PollEvents();

            if (window->IsKeyPressed(Graphic::Key::Escape)) {
                window->SetShouldClose(true);
            }
            graphics_device->Update();

            fps_counter->Update();

            ui_renderer->BeginFrame();
            text_renderer->BeginFrame();
            int winW, winH;
            window->GetFramebufferSize(winW, winH);
            ui_window->Render(*ui_renderer, *text_renderer, 0, 0, glm::vec2(winW, winH));

            ui_renderer->Render(glm::mat4(1.0f), glm::vec2(winW, winH));
            text_renderer->Render(glm::vec2(winW, winH));

            window->SwapBuffers();
        }
    }

    auto App::Shutdown() -> void {
        window->Close();
    }

    auto App::InitCoreSystem() -> void {
        Funccia::Core::AssetController::Instance().Initialize(100 * 1024 * 1024);
        Funccia::Core::ObjectPoolManager::AssetPool = new Funccia::Core::ObjectPool<Funccia::Core::Asset>();
    }

    auto App::InitFpsCounter() -> void {
        fps_counter = std::make_unique<Funccia::Util::FpsCounter>();
    }

    auto App::InitGraphicsDevice() -> void {
        graphics_device = std::make_unique<Funccia::Graphic::GL::GLGraphicsDevice>();
        graphics_device->Init();
    }

    auto App::InitUIRenderer() -> void {
        ui_renderer = std::make_unique<Funccia::Graphic::GL::UIRenderer>();
        ui_renderer->Initialize("/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/vertex/UIShader-ver.glsl",
                                "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/fragment/UIShader-frag.glsl");
    }

    auto App::InitUIWindow() -> void {
        ui_window = std::make_unique<Funccia::UI::Window>(window);


        Funccia::UI::JsonUiBuilder::ReadComponentSchema(
            "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/test.component-schema.json");
        Funccia::UI::JsonUiBuilder::ReadStyleSheet(
            "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/test.style.json");
        ui_window->ReloadFromJSON("/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/test.funccia-ui.json");
    }

    auto App::InitTextRenderer() -> void {
        text_renderer = std::make_unique<Funccia::Graphic::GL::TextRenderer>();
        text_renderer->Initialize(
            "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/vertex/FontShader1-ver.glsl",
            "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/fragment/FontShader1-fag.glsl");
    }

    auto App::InitFileWatcher() -> void {
        file_watcher = std::make_unique<Funccia::Core::FileWatcher>();
        file_watcher->Add("/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/test.funccia-ui.json",
                          [&](const std::filesystem::path& path) {
                              ui_window->ReloadFromJSON(path.string());
                          }
        );
        file_watcher->Add("/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/test.component-schema.json",
                          [&](const std::filesystem::path& path) {
                              Funccia::UI::JsonUiBuilder::ReadComponentSchema(path.string());
                              ui_window->ReloadFromJSON(
                                  "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/test.funccia-ui.json");
                          }
        );
        file_watcher->Add("/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/test.style.json",
                          [&](const std::filesystem::path& path) {
                              Funccia::UI::JsonUiBuilder::ReadStyleSheet(path.string());
                              ui_window->ReloadFromJSON("/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/test.funccia-ui.json");
                          }
        );
        file_watcher->Start();
    }
} // Funccia
