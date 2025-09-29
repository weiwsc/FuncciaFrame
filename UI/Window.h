//
// Created by Wangsicong Wei on 2025-09-26.
//

#ifndef FUNCCIAFRAME_WINDOW_H
#define FUNCCIAFRAME_WINDOW_H
#include "UIElement.h"

namespace Funccia::UI {
    class Window {
    public:
        Window();
        virtual ~Window(){};
        auto Render(Graphic::GL::UIRenderer& renderer, Graphic::GL::TextRenderer& text_render,float target_x = 0, float target_y = 0 ,const glm::vec2& screenSize = {1920, 1080}) -> void;
    private:
        std::unique_ptr<UIElement> root;
    };
}
#endif //FUNCCIAFRAME_WINDOW_H