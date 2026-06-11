//
// Created by Wangsicong Wei on 2025-09-26.
//

#include "Window.h"
#include "UIElement.h"
#include <chrono>
#include <algorithm>
#include <fstream>
#include "JsonUiBuilder.h"
#include "../graphic/WindowInterface.h"
#include "../input/MouseState.h"
#include "../core/ProfileTimer.h"

struct Timer {
    std::chrono::high_resolution_clock::time_point start;
    const char *name;

    Timer(const char *n) : name(n), start(std::chrono::high_resolution_clock::now()) {
    }

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


    auto Window::Render(Graphic::IUiRenderer &renderer, Graphic::ITextRenderer &text_renderer, float target_x,
                        float target_y, const glm::vec2 &screenSize) -> void {
            {
            PROFILE_SCOPE("layout::fit_x");
            root->CalculateFitSizeOnAxis(Axis::Horizontal);
            }
            {
                PROFILE_SCOPE("layout::grow_x");
                root->CalculateGrowSizeOnAxis(Axis::Horizontal, text_renderer);
            }
            {
                PROFILE_SCOPE("layout::fit_y");
                root->CalculateFitSizeOnAxis(Axis::Vertical);
            }
            {
                PROFILE_SCOPE("layout::grow_y");
                root->CalculateGrowSizeOnAxis(Axis::Vertical, text_renderer);
            }
            {
                PROFILE_SCOPE("layout::pos_x");
                root->PositionOnAxis(Axis::Horizontal, m_offset.x);
            }
            {
                PROFILE_SCOPE("layout::pos_y");
                root->PositionOnAxis(Axis::Vertical, m_offset.y);
            }


        double x, y;
        {
            PROFILE_SCOPE("position");
            m_window->GetCursorPos(x, y);
            //target_x = 2*x;
            //target_y = 2*y;
            root->GlobalPositionPass(target_x, target_y);
            root->CullingPass(vec4(0, 0, screenSize.x, screenSize.y));
        }

        {
            PROFILE_SCOPE("input");
            auto* mouse = m_window->Mouse();
            if (mouse && mouse->GetButLDown()) {
                root->HandlePick({x, y}, this);
            }
            //SetHoveredElement(nullptr);
            if (!root->HandleHover({x, y}, this)) { SetHoveredElement(nullptr); };
            if (selected_element) {
                selected_element->GetState().Set(UIFlag::Selected, selected_element->Style());
            }
            if (mouse && mouse->HasScrolled()) {
                if (UIElement* handler = root->FindScrollHandler({x, y})) {
                    vec2 scrolled = mouse->GetScroll();
                    float height = 0;
                    for (auto& child : handler->m_children) {
                        height += child->m_border_box_size.GetY();
                    }
                    handler->m_scroll.y = std::min((scrolled.y)*5.0f + (handler->m_scroll.y), 0.0f);

                    handler->m_scroll.x = std::min(scrolled.x + handler->m_scroll.x, 0.0f);
                    mouse->ScrollHandled();
                }
            }
        }
        {
            PROFILE_SCOPE("submit");
            root->RenderQueue(renderer, text_renderer, target_x, target_y, m_window->GetDisplayScale());
        }

    }

    auto Window::RenderProfile(Graphic::IUiRenderer &renderer, Graphic::ITextRenderer &text_renderer,
                               Graphic::ITextRenderer &text_render,
                               float target_x, float target_y, const glm::vec2 &screenSize) -> void {
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
        root->RenderQueue(renderer, text_renderer, target_x, target_y, m_window->GetDisplayScale());
        end = std::chrono::high_resolution_clock::now();
        double renderQueue = std::chrono::duration<double, std::milli>(end - start).count();

        printf(
            "FitX: %.3f | GrowX: %.3f | FitY: %.3f | GrowY: %.3f | PosX: %.3f | PosY: %.3f | Global: %.3f | Cull: %.3f | Queue: %.3f\n",
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


    auto Window::InitLayout(float target_x, float target_y, const glm::vec2 &screenSize) -> void {
        //root->Scale(2);
        root->GlobalPositionPass(target_x, target_y);
        root->CullingPass(vec4(0, 0, screenSize.x, screenSize.y));
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
