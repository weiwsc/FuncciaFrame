//
// Created by Wangsicong Wei on 2026-01-14.
//

#pragma once
#include <memory>

#include "ApplicationMetadata.h"

namespace Funccia {
    namespace Util {
        class FpsCounter;
    }

    namespace Core {
        class FileWatcher;
    }

    namespace UI {
        class Window;
    }

    namespace Graphic {
        enum class RenderBackend;
        class GraphicsDevice;
        class ITextRenderer;
        class IUiRenderer;
        class WindowInterface;
    }

    class App : public Core::Singleton<App> {
    public:
        App();
        ~App() override;
        auto Init() -> void;
        auto Update() -> void;
        auto Shutdown() -> void;
        ApplicationMetadata application_metadata;
    private:
        auto InitCoreSystem() -> void;
        auto InitWindow(Graphic::RenderBackend backend) -> void;
        auto InitFpsCounter() -> void;
        auto InitGraphicsDevice() -> void;
        auto InitUIRenderer() -> void;
        auto InitUIWindow() -> void;
        auto InitTextRenderer() -> void;
        auto InitFileWatcher() -> void;

        std::unique_ptr<Funccia::Graphic::IUiRenderer> ui_renderer;
        std::unique_ptr<Funccia::Graphic::ITextRenderer> text_renderer;
        std::unique_ptr<Funccia::UI::Window> ui_window;
        std::unique_ptr<Funccia::Core::FileWatcher> file_watcher;
        std::unique_ptr<Funccia::Graphic::GraphicsDevice> graphics_device;
        std::unique_ptr<Funccia::Util::FpsCounter> fps_counter;
        Graphic::WindowInterface* window = nullptr;


    };
} // Funccia
