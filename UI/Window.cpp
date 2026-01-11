//
// Created by Wangsicong Wei on 2025-09-26.
//

#include "Window.h"
#include "UIElement.h"
#include <chrono>

#include "JsonUiBuilder.h"
#include "../graphic/WindowInterface.h"
#include "../graphic/gl/TextRenderer.h"
#include "../graphic/gl/WindowController.h"

struct Timer {
    std::chrono::high_resolution_clock::time_point start;
    const char* name;

    Timer(const char* n) : name(n), start(std::chrono::high_resolution_clock::now()) {}

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        printf("%s: %.3f ms\n", name, ms);
    }
};
namespace Funccia::UI {



    Window::Window(Graphic::WindowInterface *window) {
        m_window = window;
        //TextUI();
        //TestUI();
        //MockUI();
        root = std::make_unique<UIElement>();

    }


    auto Window::Render(Graphic::GL::UIRenderer& renderer, Graphic::GL::TextRenderer& text_renderer, float target_x, float target_y, const glm::vec2& screenSize) -> void {
        root->CalculateFitSizeOnAxis(Axis::Horizontal);
        root->CalculateGrowSizeOnAxis(Axis::Horizontal, text_renderer);

        root->CalculateFitSizeOnAxis(Axis::Vertical);
        root->CalculateGrowSizeOnAxis(Axis::Vertical, text_renderer);

        root->PositionOnAxis(Axis::Horizontal, m_offset.x);
        root->PositionOnAxis(Axis::Vertical, m_offset.y);

        double x,y;
        m_window->GetCursorPos(x, y);
        //target_x = 2*x;
        //target_y = 2*y;
        root->GlobalPositionPass(target_x, target_y);
        root->CullingPass(vec4(0 , 0, screenSize.x, screenSize.y));

        if (Graphic::GL::WindowController::Instance().GetWindow()->Mouse()->GetButLDown()) {
            root->HandlePick({x*2,y*2}, this);
        }
        //SetHoveredElement(nullptr);
        if (!root->HandleHover({x*2, y*2}, this)) {SetHoveredElement(nullptr);};
        if (selected_element) {
            selected_element->GetState().Set(UIFlag::Selected, selected_element->Style());
        }

        root->RenderQueue(renderer,text_renderer, target_x, target_y);

        //m_offset.y += 0.7;
        //m_offset.y += 0.01;
        //root->GlobalPositionPass(root->borderBoxEndOnAxis(Axis::Horizontal) + 0.01, root->borderBoxEndOnAxis(Axis::Vertical)+ 0.01);
        //root->MarginLeft((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
        //root->MarginRight((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);


    }

    auto Window::RenderProfile(Graphic::GL::UIRenderer& renderer,Graphic::GL::TextRenderer& text_renderer,  Graphic::GL::TextRenderer& text_render,
                    float target_x, float target_y, const glm::vec2& screenSize) -> void {

    auto start = std::chrono::high_resolution_clock::now();
    root->CalculateFitSizeOnAxis(Axis::Horizontal);
    auto end = std::chrono::high_resolution_clock::now();
    double fitX = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    root->CalculateGrowSizeOnAxis(Axis::Horizontal, text_renderer);
    end = std::chrono::high_resolution_clock::now();
    double growX = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    root->CalculateFitSizeOnAxis(Axis::Vertical);
    end = std::chrono::high_resolution_clock::now();
    double fitY = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    root->CalculateGrowSizeOnAxis(Axis::Vertical, text_renderer);
    end = std::chrono::high_resolution_clock::now();
    double growY = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    root->PositionOnAxis(Axis::Horizontal, 0);
    end = std::chrono::high_resolution_clock::now();
    double posX = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    root->PositionOnAxis(Axis::Vertical, 0);
    end = std::chrono::high_resolution_clock::now();
    double posY = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    root->GlobalPositionPass(target_x, target_y);
    end = std::chrono::high_resolution_clock::now();
    double globalPos = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    root->CullingPass(vec4(0, 0, screenSize.x, screenSize.y));
    end = std::chrono::high_resolution_clock::now();
    double culling = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    root->RenderQueue(renderer,text_renderer, target_x, target_y);
    end = std::chrono::high_resolution_clock::now();
    double renderQueue = std::chrono::duration<double, std::milli>(end - start).count();

    printf("FitX: %.3f | GrowX: %.3f | FitY: %.3f | GrowY: %.3f | PosX: %.3f | PosY: %.3f | Global: %.3f | Cull: %.3f | Queue: %.3f\n",
           fitX, growX, fitY, growY, posX, posY, globalPos, culling, renderQueue);

    //root->MarginLeft((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
    //root->MarginRight((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
}

    auto Window::ReloadFromJSON(const std::string &json_path) const -> void {
        std::ifstream file(json_path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        root->m_children.clear();
        Funccia::UI::JsonUiBuilder::ParseUI(content, root.get());
    }

    auto Window::SetHoveredElement(UIElement *element) -> void {
        if (hovered_element == element) return;
        //std::cout << "SetHoveredElement called" << std::endl;
        //std::cout << "  new: " << element << std::endl;
        //std::cout << "  old: " << hovered_element << std::endl;

        if (hovered_element) {
            //std::cout << "  clearing old" << std::endl;
            hovered_element->GetState().Clear(UIFlag::Hovered, hovered_element->Style());
        }
        hovered_element = element;
        if (hovered_element) {
            //std::cout << "  setting new" << std::endl;
            hovered_element->GetState().Set(UIFlag::Hovered, hovered_element->Style());
        }
    }


    auto Window::InitLayout(float target_x, float target_y, const glm::vec2& screenSize) -> void {
        //root->Scale(2);
        root->GlobalPositionPass(target_x, target_y);
        root->CullingPass(vec4(0 , 0, screenSize.x, screenSize.y));
        //root->MarginLeft((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
        //root->MarginRight((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
        root->CalculateFitSizeOnAxis(Axis::Horizontal);
        //root->CalculateGrowSizeOnAxis(Axis::Horizontal);

        root->CalculateFitSizeOnAxis(Axis::Vertical);
        //root->CalculateGrowSizeOnAxis(Axis::Vertical);

        root->PositionOnAxis(Axis::Horizontal, 0);
        root->PositionOnAxis(Axis::Vertical, 0);
#ifdef FF_UI_LAZY_LAYOUT
        root->FlipLazyLayoutPass();
#endif
    }

}
