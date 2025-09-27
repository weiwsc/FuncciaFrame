//
// Created by Wangsicong Wei on 2025-09-26.
//

#include "Window.h"


namespace Funccia::UI {
Window::Window() {
    // Create a toolbar with buttons
    auto btn1 = std::make_unique<UIElement>();
    btn1->m_margin = {5, 5, 5, 5};
    btn1->m_padding = {8, 12, 8, 12};
    btn1->m_background = {0.2, 0.6, 1.0, 1};  // Light blue
    btn1->m_border_radius = {4, 4, 4, 4};
    btn1->m_sizing.x_type = SizingType::Fixed;
    btn1->m_sizing.y_type = SizingType::Fixed;
    btn1->m_sizing.x_value = 80;
    btn1->m_sizing.y_value = 32;

    auto btn2 = std::make_unique<UIElement>();
    btn2->m_margin = {5, 5, 5, 5};
    btn2->m_padding = {8, 12, 8, 12};
    btn2->m_background = {0.8, 0.4, 0.2, 1};  // Orange
    btn2->m_border_radius = {4, 4, 4, 4};
    btn2->m_sizing.x_type = SizingType::Fixed;
    btn2->m_sizing.y_type = SizingType::Fixed;
    btn2->m_sizing.x_value = 100;
    btn2->m_sizing.y_value = 32;

    auto btn3 = std::make_unique<UIElement>();
    btn3->m_margin = {5, 5, 5, 5};
    btn3->m_padding = {8, 12, 8, 12};
    btn3->m_background = {0.6, 0.2, 0.8, 1};  // Purple
    btn3->m_border_radius = {4, 4, 4, 4};
    btn3->m_sizing.x_type = SizingType::Fixed;
    btn3->m_sizing.y_type = SizingType::Fixed;
    btn3->m_sizing.x_value = 90;
    btn3->m_sizing.y_value = 32;

    auto toolbar = std::make_unique<UIElement>();
    toolbar->m_margin = {0, 0, 10, 0};  // Margin bottom
    toolbar->m_padding = {8, 8, 8, 8};
    toolbar->m_background = {0.9, 0.9, 0.9, 1};  // Light gray
    toolbar->m_displayAxis = Axis::Horizontal;
    toolbar->AddChild(std::move(btn1));
    toolbar->AddChild(std::move(btn2));
    toolbar->AddChild(std::move(btn3));

    // Create a sidebar with nested content
    auto sidebarItem1 = std::make_unique<UIElement>();
    sidebarItem1->m_margin = {5, 5, 5, 5};
    sidebarItem1->m_padding = {10, 10, 10, 10};
    sidebarItem1->m_background = {0.3, 0.7, 0.3, 1};  // Green
    sidebarItem1->m_border_radius = {6, 6, 6, 6};
    sidebarItem1->m_sizing.x_type = SizingType::Fixed;
    sidebarItem1->m_sizing.y_type = SizingType::Fixed;
    sidebarItem1->m_sizing.x_value = 140;
    sidebarItem1->m_sizing.y_value = 60;

    auto sidebarItem2 = std::make_unique<UIElement>();
    sidebarItem2->m_margin = {5, 5, 5, 5};
    sidebarItem2->m_padding = {10, 10, 10, 10};
    sidebarItem2->m_background = {0.7, 0.3, 0.3, 1};  // Red
    sidebarItem2->m_border_radius = {6, 6, 6, 6};
    sidebarItem2->m_sizing.x_type = SizingType::Fixed;
    sidebarItem2->m_sizing.y_type = SizingType::Fixed;
    sidebarItem2->m_sizing.x_value = 140;
    sidebarItem2->m_sizing.y_value = 80;

    // Nested container inside sidebar item
    auto nestedBox = std::make_unique<UIElement>();
    nestedBox->m_margin = {8, 8, 8, 8};
    nestedBox->m_background = {1.0, 1.0, 0.3, 1};  // Yellow
    nestedBox->m_border_radius = {3, 3, 3, 3};
    nestedBox->m_sizing.x_type = SizingType::Fixed;
    nestedBox->m_sizing.y_type = SizingType::Fixed;
    nestedBox->m_sizing.x_value = 60;
    nestedBox->m_sizing.y_value = 30;

    sidebarItem2->AddChild(std::move(nestedBox));

    auto sidebar = std::make_unique<UIElement>();
    sidebar->m_margin = {5, 0, 5, 0};  // Margin right
    sidebar->m_padding = {10, 10, 10, 10};
    sidebar->m_background = {0.85, 0.85, 0.9, 1};  // Light purple
    sidebar->m_displayAxis = Axis::Vertical;
    sidebar->AddChild(std::move(sidebarItem1));
    sidebar->AddChild(std::move(sidebarItem2));

    // Create main content area with cards
    auto card1 = std::make_unique<UIElement>();
    card1->m_margin = {10, 10, 10, 10};
    card1->m_padding = {15, 15, 15, 15};
    card1->m_background = {1.0, 1.0, 1.0, 1};  // White
    card1->m_border_radius = {8, 8, 8, 8};
    card1->m_shadow = {{0.5, 0.5, 0.5, 0.3}, {2, 2}, 8, 0};  // Drop shadow
    card1->m_sizing.x_type = SizingType::Fixed;
    card1->m_sizing.y_type = SizingType::Fixed;
    card1->m_sizing.x_value = 200;
    card1->m_sizing.y_value = 120;

    auto card2 = std::make_unique<UIElement>();
    card2->m_margin = {10, 10, 10, 10};
    card2->m_padding = {15, 15, 15, 15};
    card2->m_background = {0.95, 0.98, 1.0, 1};  // Very light blue
    card2->m_border_radius = {8, 8, 8, 8};
    card2->m_shadow = {{0.5, 0.5, 0.5, 0.3}, {2, 2}, 8, 0};
    card2->m_sizing.x_type = SizingType::Fixed;
    card2->m_sizing.y_type = SizingType::Fixed;
    card2->m_sizing.x_value = 180;
    card2->m_sizing.y_value = 100;

    // Content inside card1
    auto cardContent = std::make_unique<UIElement>();
    cardContent->m_margin = {5, 5, 5, 5};
    cardContent->m_background = {0.2, 0.8, 0.9, 1};  // Cyan
    cardContent->m_border_radius = {4, 4, 4, 4};
    cardContent->m_sizing.x_type = SizingType::Fixed;
    cardContent->m_sizing.y_type = SizingType::Fixed;
    cardContent->m_sizing.x_value = 120;
    cardContent->m_sizing.y_value = 40;

    card1->AddChild(std::move(cardContent));

    auto mainContent = std::make_unique<UIElement>();
    mainContent->m_margin = {5, 0, 5, 0};  // Margin between sidebar and main content
    mainContent->m_padding = {15, 15, 15, 15};
    mainContent->m_background = {0.98, 0.98, 0.98, 1};  // Very light gray
    mainContent->m_displayAxis = Axis::Vertical;
    mainContent->AddChild(std::move(card1));
    mainContent->AddChild(std::move(card2));

    // Create body container (horizontal layout)
    auto body = std::make_unique<UIElement>();
    body->m_margin = {0, 0, 0, 0};
    body->m_padding = {0, 0, 0, 0};
    body->m_background = {0.1, 0.1, 1, 0};  // Dark background
    body->m_displayAxis = Axis::Horizontal;
    body->AddChild(std::move(sidebar));
    body->AddChild(std::move(mainContent));

    // Root container (vertical layout)
    root = std::make_unique<UIElement>();
    root->m_margin = {20, 20, 20, 20};  // Margin from screen edges
    root->m_padding = {12, 12, 12, 12};
    root->m_background = {0.1, 0.1, 0.15, 1};  // Dark background
    root->m_border_radius = {0, 12, 0, 12};
    root->m_displayAxis = Axis::Vertical;
    root->AddChild(std::move(toolbar));
    root->AddChild(std::move(body));
}



    auto Window::Render(Graphic::GL::UIRenderer& renderer, float target_x, float target_y) -> void {
        root->CalculateFitSizeOnAxis(Axis::Horizontal);
        root->CalculateFitSizeOnAxis(Axis::Vertical);
        root->PositionOnAxis(Axis::Horizontal, 0);
        root->PositionOnAxis(Axis::Vertical, 0);
        root->RenderQueue(renderer, target_x, target_y);
    }

}
