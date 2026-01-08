//
// Created by Wangsicong Wei on 2025-09-26.
//

#ifndef FUNCCIAFRAME_WINDOW_H
#define FUNCCIAFRAME_WINDOW_H
#include "UIElement.h"
#include "../graphic/WindowInterface.h"

namespace Funccia::UI {
    class Window {
    public:
        Window(Graphic::WindowInterface* window);
        virtual ~Window(){};
        auto InitLayout(float target_x, float target_y, const glm::vec2& screenSize) -> void;
        auto UI1() -> void;
        auto UI2() -> void;
        auto UI3() -> void;
        auto TextUI()->void;
        auto TestUI()->void;
        auto MockUI() -> void;
        auto ModernUITest() -> void;
        auto UIStressTest() -> void;
        auto Render(Graphic::GL::UIRenderer& renderer,Graphic::GL::TextRenderer& text_renderer, float target_x = 0, float target_y = 0 ,const glm::vec2& screenSize = {1920, 1080}) -> void;
        auto RenderProfile(Graphic::GL::UIRenderer& renderer,Graphic::GL::TextRenderer& text_renderer,  Graphic::GL::TextRenderer& text_render,float target_x = 0, float target_y = 0 ,const glm::vec2& screenSize = {1920, 1080}) -> void;
    private:
        std::unique_ptr<UIElement> root;
        vec2 m_offset = {0,0};
        Graphic::WindowInterface* m_window;
    };
}
#endif //FUNCCIAFRAME_WINDOW_H