//
// Created by Wangsicong Wei on 2026-01-14.
//

#pragma once
#include <memory>

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
        class GraphicsDevice;
        class WindowInterface;
    }

    namespace Graphic::GL {
        class TextRenderer;
        class UIRenderer;
    }

    class App {
    public:
        App();
        virtual ~App();
        auto Init() -> void;
        auto Update() -> void;
        auto Shutdown() -> void;
    private:
        auto InitCoreSystem() -> void;
        auto InitFpsCounter() -> void;
        auto InitGraphicsDevice() -> void;
        auto InitUIRenderer() -> void;
        auto InitUIWindow() -> void;
        auto InitTextRenderer() -> void;
        auto InitFileWatcher() -> void;

        std::unique_ptr<Funccia::Graphic::GL::UIRenderer> ui_renderer;
        std::unique_ptr<Funccia::Graphic::GL::TextRenderer> text_renderer;
        std::unique_ptr<Funccia::UI::Window> ui_window;
        std::unique_ptr<Funccia::Core::FileWatcher> file_watcher;
        std::unique_ptr<Funccia::Graphic::GraphicsDevice> graphics_device;
        std::unique_ptr<Funccia::Util::FpsCounter> fps_counter;
        Graphic::WindowInterface* window = nullptr;


    };
} // Funccia