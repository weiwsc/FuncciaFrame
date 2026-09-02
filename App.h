//
// Created by Wangsicong Wei on 2026-01-14.
//

#pragma once
#include <memory>

#include "ApplicationMetadata.h"

namespace vva {
    namespace util {
        class FpsCounter;
    }

    namespace core {
        class FileWatcher;
    }

    namespace UI {
        class Window;
    }

    namespace gfx {
        enum class RenderBackend;
        class GraphicsDevice;
        class ITextRenderer;
        class IUiRenderer;
        class WindowInterface;
    }

    class App : public core::Singleton<App> {
    public:
        App();
        ~App() override;
        auto Init() -> void;
        auto Update() -> void;
        auto Shutdown() -> void;
        ApplicationMetadata application_metadata;
    private:
        auto InitCoreSystem() -> void;
        auto InitWindow(gfx::RenderBackend backend) -> void;
        auto InitFpsCounter() -> void;
        auto InitGraphicsDevice() -> void;
        auto InitUIRenderer() -> void;
        auto InitUIWindow() -> void;
        auto InitTextRenderer() -> void;
        auto InitFileWatcher() -> void;

        std::unique_ptr<vva::gfx::IUiRenderer> ui_renderer;
        std::unique_ptr<vva::gfx::ITextRenderer> text_renderer;
        std::unique_ptr<vva::UI::Window> ui_window;
        std::unique_ptr<vva::core::FileWatcher> file_watcher;
        std::unique_ptr<vva::gfx::GraphicsDevice> graphics_device;
        std::unique_ptr<vva::util::FpsCounter> fps_counter;
        gfx::WindowInterface* window = nullptr;


    };
} // Funccia
