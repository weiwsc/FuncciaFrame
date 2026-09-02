//
// Created by Wangsicong Wei on 2025-09-26.
//

#ifndef FUNCCIAFRAME_WINDOW_H
#define FUNCCIAFRAME_WINDOW_H
#include "UIElement.h"
#include "../graphic/WindowInterface.h"

namespace vva::UI {
    class Window {
    public:
        Window(gfx::WindowInterface* window);
        virtual ~Window(){};
        auto InitLayout(float target_x, float target_y, const glm::vec2& screenSize) -> void;

        auto Render(gfx::IUiRenderer& renderer,gfx::ITextRenderer& text_renderer, float target_x = 0, float target_y = 0 ,const glm::vec2& screenSize = {1920, 1080}) -> void;
        auto RenderProfile(gfx::IUiRenderer& renderer,gfx::ITextRenderer& text_renderer,  gfx::ITextRenderer& text_render,float target_x = 0, float target_y = 0 ,const glm::vec2& screenSize = {1920, 1080}) -> void;
        UIElement* hovered_element {nullptr};
        UIElement* selected_element {nullptr};
        [[nodiscard]] auto GetRoot() const -> UIElement * {return root.get();}
        auto ReloadFromJSON(const std::string& json_path) const -> void;

        auto SetHoveredElement(UIElement* element) -> void;
    private:
        std::unique_ptr<UIElement> root;
        vec2 m_offset = {0,0};
        gfx::WindowInterface* m_window;


    };
}
#endif //FUNCCIAFRAME_WINDOW_H