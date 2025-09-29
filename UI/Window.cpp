//
// Created by Wangsicong Wei on 2025-09-26.
//

#include "Window.h"
#include "UIElement.h"

namespace Funccia::UI {
// Window::Window() {
//     // Header Navigation
//     auto logo = std::make_unique<UIElement>();
//     logo->m_margin = {12, 8, 12, 8};
//     logo->m_background = {1.0, 1.0, 1.0, 1};
//     logo->m_border_radius = {8, 8, 8, 8};
//     logo->m_border_widths = {2, 2, 2, 2};
//     logo->m_border_color = {0.898, 0.906, 0.922, 1}; // #e5e7eb
//     logo->m_sizing.x_type = SizingType::Fixed;
//     logo->m_sizing.y_type = SizingType::Fixed;
//     logo->m_sizing.x_value = 36;
//     logo->m_sizing.y_value = 36;
//
//     auto navHome = std::make_unique<UIElement>();
//     navHome->m_margin = {4, 4, 4, 4};
//     navHome->m_padding = {8, 16, 8, 16};
//     navHome->m_background = {1.0, 1.0, 1.0, 0.1};
//     navHome->m_border_radius = {6, 6, 6, 6};
//     navHome->m_border_widths = {1, 1, 1, 1};
//     navHome->m_border_color = {1.0, 1.0, 1.0, 0.2};
//     navHome->m_sizing.x_type = SizingType::Fixed;
//     navHome->m_sizing.y_type = SizingType::Fixed;
//     navHome->m_sizing.x_value = 60;
//     navHome->m_sizing.y_value = 36;
//
//     auto navDashboard = std::make_unique<UIElement>();
//     navDashboard->m_margin = {4, 4, 4, 4};
//     navDashboard->m_padding = {8, 16, 8, 16};
//     navDashboard->m_background = {1.0, 1.0, 1.0, 0.2}; // Active state
//     navDashboard->m_border_radius = {6, 6, 6, 6};
//     navDashboard->m_border_widths = {1, 1, 1, 1};
//     navDashboard->m_border_color = {1.0, 1.0, 1.0, 0.3};
//     navDashboard->m_sizing.x_type = SizingType::Fixed;
//     navDashboard->m_sizing.y_type = SizingType::Fixed;
//     navDashboard->m_sizing.x_value = 80;
//     navDashboard->m_sizing.y_value = 36;
//
//     auto navSettings = std::make_unique<UIElement>();
//     navSettings->m_margin = {4, 4, 4, 4};
//     navSettings->m_padding = {8, 16, 8, 16};
//     navSettings->m_background = {1.0, 1.0, 1.0, 0.1};
//     navSettings->m_border_radius = {6, 6, 6, 6};
//     navSettings->m_border_widths = {1, 1, 1, 1};
//     navSettings->m_border_color = {1.0, 1.0, 1.0, 0.2};
//     navSettings->m_sizing.x_type = SizingType::Fixed;
//     navSettings->m_sizing.y_type = SizingType::Fixed;
//     navSettings->m_sizing.x_value = 70;
//     navSettings->m_sizing.y_value = 36;
//
//     auto navProfile = std::make_unique<UIElement>();
//     navProfile->m_margin = {4, 4, 4, 4};
//     navProfile->m_padding = {8, 16, 8, 16};
//     navProfile->m_background = {1.0, 1.0, 1.0, 0.1};
//     navProfile->m_border_radius = {6, 6, 6, 6};
//     navProfile->m_border_widths = {1, 1, 1, 1};
//     navProfile->m_border_color = {1.0, 1.0, 1.0, 0.2};
//     navProfile->m_sizing.x_type = SizingType::Fixed;
//     navProfile->m_sizing.y_type = SizingType::Fixed;
//     navProfile->m_sizing.x_value = 60;
//     navProfile->m_sizing.y_value = 36;
//
//     auto header = std::make_unique<UIElement>();
//     header->m_padding = {12, 20, 12, 20};
//     header->m_background = {0.149, 0.388, 0.922, 1}; // #2563eb
//     header->m_border_widths = {0, 0, 3, 0};
//     header->m_border_color = {0.114, 0.306, 0.847, 1}; // #1d4ed8
//     header->m_displayAxis = Axis::Horizontal;
//     header->m_sizing.x_type = SizingType::Fixed;
//     header->m_sizing.y_type = SizingType::Fixed;
//     header->m_sizing.x_value = 960;
//     header->m_sizing.y_value = 60;
//     header->AddChild(std::move(logo));
//     header->AddChild(std::move(navHome));
//     header->AddChild(std::move(navDashboard));
//     header->AddChild(std::move(navSettings));
//     header->AddChild(std::move(navProfile));
//
//     // Sidebar Section 1
//     auto sidebarTitle1 = std::make_unique<UIElement>();
//     sidebarTitle1->m_margin = {0, 0, 12, 0};
//     sidebarTitle1->m_background = {0.392, 0.455, 0.545, 1}; // #64748b
//     sidebarTitle1->m_border_radius = {4, 4, 4, 4};
//     sidebarTitle1->m_border_widths = {1, 1, 1, 1};
//     sidebarTitle1->m_border_color = {0.278, 0.333, 0.427, 1}; // #475569
//     sidebarTitle1->m_sizing.x_type = SizingType::Fixed;
//     sidebarTitle1->m_sizing.y_type = SizingType::Fixed;
//     sidebarTitle1->m_sizing.x_value = 176;
//     sidebarTitle1->m_sizing.y_value = 24;
//
//     auto sidebarItem1 = std::make_unique<UIElement>();
//     sidebarItem1->m_margin = {0, 0, 8, 0};
//     sidebarItem1->m_background = {0.231, 0.506, 0.965, 1}; // #3b82f6 (active)
//     sidebarItem1->m_border_radius = {6, 6, 6, 6};
//     sidebarItem1->m_border_widths = {1, 1, 1, 1};
//     sidebarItem1->m_border_color = {0.149, 0.388, 0.922, 1}; // #2563eb
//     sidebarItem1->m_sizing.x_type = SizingType::Fixed;
//     sidebarItem1->m_sizing.y_type = SizingType::Fixed;
//     sidebarItem1->m_sizing.x_value = 176;
//     sidebarItem1->m_sizing.y_value = 32;
//
//     auto sidebarItem2 = std::make_unique<UIElement>();
//     sidebarItem2->m_margin = {0, 0, 8, 0};
//     sidebarItem2->m_background = {0.886, 0.910, 0.941, 1}; // #e2e8f0
//     sidebarItem2->m_border_radius = {6, 6, 6, 6};
//     sidebarItem2->m_border_widths = {1, 1, 1, 1};
//     sidebarItem2->m_border_color = {0.796, 0.835, 0.882, 1}; // #cbd5e1
//     sidebarItem2->m_sizing.x_type = SizingType::Fixed;
//     sidebarItem2->m_sizing.y_type = SizingType::Fixed;
//     sidebarItem2->m_sizing.x_value = 176;
//     sidebarItem2->m_sizing.y_value = 32;
//
//     auto sidebarItem3 = std::make_unique<UIElement>();
//     sidebarItem3->m_background = {0.886, 0.910, 0.941, 1}; // #e2e8f0
//     sidebarItem3->m_border_radius = {6, 6, 6, 6};
//     sidebarItem3->m_border_widths = {1, 1, 1, 1};
//     sidebarItem3->m_border_color = {0.796, 0.835, 0.882, 1}; // #cbd5e1
//     sidebarItem3->m_sizing.x_type = SizingType::Fixed;
//     sidebarItem3->m_sizing.y_type = SizingType::Fixed;
//     sidebarItem3->m_sizing.x_value = 176;
//     sidebarItem3->m_sizing.y_value = 32;
//
//     auto sidebarSection1 = std::make_unique<UIElement>();
//     sidebarSection1->m_margin = {0, 0, 12, 0};
//     sidebarSection1->m_padding = {16, 16, 16, 16};
//     sidebarSection1->m_background = {1.0, 1.0, 1.0, 1};
//     sidebarSection1->m_border_radius = {8, 8, 8, 8};
//     sidebarSection1->m_border_widths = {1, 1, 1, 1};
//     sidebarSection1->m_border_color = {0.796, 0.835, 0.882, 1}; // #cbd5e1
//     sidebarSection1->m_shadow = {{0.0, 0.0, 0.0, 0.05}, {0, 2}, 8, 0};
//     sidebarSection1->m_displayAxis = Axis::Vertical;
//     sidebarSection1->AddChild(std::move(sidebarTitle1));
//     sidebarSection1->AddChild(std::move(sidebarItem1));
//     sidebarSection1->AddChild(std::move(sidebarItem2));
//     sidebarSection1->AddChild(std::move(sidebarItem3));
//
//     // Sidebar Section 2
//     auto sidebarTitle2 = std::make_unique<UIElement>();
//     sidebarTitle2->m_margin = {0, 0, 12, 0};
//     sidebarTitle2->m_background = {0.392, 0.455, 0.545, 1}; // #64748b
//     sidebarTitle2->m_border_radius = {4, 4, 4, 4};
//     sidebarTitle2->m_border_widths = {1, 1, 1, 1};
//     sidebarTitle2->m_border_color = {0.278, 0.333, 0.427, 1}; // #475569
//     sidebarTitle2->m_sizing.x_type = SizingType::Fixed;
//     sidebarTitle2->m_sizing.y_type = SizingType::Fixed;
//     sidebarTitle2->m_sizing.x_value = 176;
//     sidebarTitle2->m_sizing.y_value = 24;
//
//     auto sidebarItem4 = std::make_unique<UIElement>();
//     sidebarItem4->m_margin = {0, 0, 8, 0};
//     sidebarItem4->m_background = {0.886, 0.910, 0.941, 1}; // #e2e8f0
//     sidebarItem4->m_border_radius = {6, 6, 6, 6};
//     sidebarItem4->m_border_widths = {1, 1, 1, 1};
//     sidebarItem4->m_border_color = {0.796, 0.835, 0.882, 1}; // #cbd5e1
//     sidebarItem4->m_sizing.x_type = SizingType::Fixed;
//     sidebarItem4->m_sizing.y_type = SizingType::Fixed;
//     sidebarItem4->m_sizing.x_value = 176;
//     sidebarItem4->m_sizing.y_value = 32;
//
//     auto sidebarItem5 = std::make_unique<UIElement>();
//     sidebarItem5->m_background = {0.886, 0.910, 0.941, 1}; // #e2e8f0
//     sidebarItem5->m_border_radius = {6, 6, 6, 6};
//     sidebarItem5->m_border_widths = {1, 1, 1, 1};
//     sidebarItem5->m_border_color = {0.796, 0.835, 0.882, 1}; // #cbd5e1
//     sidebarItem5->m_sizing.x_type = SizingType::Fixed;
//     sidebarItem5->m_sizing.y_type = SizingType::Fixed;
//     sidebarItem5->m_sizing.x_value = 176;
//     sidebarItem5->m_sizing.y_value = 32;
//
//     auto sidebarSection2 = std::make_unique<UIElement>();
//     sidebarSection2->m_padding = {16, 16, 16, 16};
//     sidebarSection2->m_background = {1.0, 1.0, 1.0, 1};
//     sidebarSection2->m_border_radius = {8, 8, 8, 8};
//     sidebarSection2->m_border_widths = {1, 1, 1, 1};
//     sidebarSection2->m_border_color = {0.796, 0.835, 0.882, 1}; // #cbd5e1
//     sidebarSection2->m_shadow = {{0.0, 0.0, 0.0, 0.05}, {0, 2}, 8, 0};
//     sidebarSection2->m_displayAxis = Axis::Vertical;
//     sidebarSection2->AddChild(std::move(sidebarTitle2));
//     sidebarSection2->AddChild(std::move(sidebarItem4));
//     sidebarSection2->AddChild(std::move(sidebarItem5));
//
//     auto sidebar = std::make_unique<UIElement>();
//     sidebar->m_padding = {16, 16, 16, 16};
//     sidebar->m_background = {0.973, 0.980, 0.988, 1}; // #f8fafc
//     sidebar->m_border_widths = {0, 2, 0, 0};
//     sidebar->m_border_color = {0.886, 0.910, 0.941, 1}; // #e2e8f0
//     sidebar->m_displayAxis = Axis::Vertical;
//     sidebar->m_sizing.x_type = SizingType::Fixed;
//     sidebar->m_sizing.y_type = SizingType::Fixed;
//     sidebar->m_sizing.x_value = 240;
//     sidebar->m_sizing.y_value = 580; // Total height - header
//     sidebar->AddChild(std::move(sidebarSection1));
//     sidebar->AddChild(std::move(sidebarSection2));
//
//     // Content Header
//     auto contentHeader = std::make_unique<UIElement>();
//     contentHeader->m_margin = {0, 0, 16, 0};
//     contentHeader->m_padding = {12, 16, 12, 16};
//     contentHeader->m_background = {0.945, 0.961, 0.973, 1}; // #f1f5f9
//     contentHeader->m_border_radius = {8, 8, 8, 8};
//     contentHeader->m_border_widths = {2, 2, 2, 2};
//     contentHeader->m_border_color = {0.796, 0.835, 0.882, 1}; // #cbd5e1
//     contentHeader->m_sizing.x_type = SizingType::Fixed;
//     contentHeader->m_sizing.y_type = SizingType::Fixed;
//     contentHeader->m_sizing.x_value = 680;
//     contentHeader->m_sizing.y_value = 48;
//
//     // Card 1 (Green)
//     auto card1Header = std::make_unique<UIElement>();
//     card1Header->m_margin = {0, 0, 12, 0};
//     card1Header->m_background = {0.022, 0.588, 0.412, 1}; // #059669
//     card1Header->m_border_radius = {1, 1, 1, 1};
//     card1Header->m_border_widths = {2, 2, 2, 2};
//     card1Header->m_border_color = {0.018, 0.471, 0.341, 1}; // #047857
//     card1Header->m_sizing.x_type = SizingType::Fixed;
//     card1Header->m_sizing.y_type = SizingType::Fixed;
//     card1Header->m_sizing.x_value = 188;
//     card1Header->m_sizing.y_value = 32;
//     card1Header->m_shadow = {{0.0, 0.0, 0.0, 0.3}, {0, 2}, 8, 0};
//
//     auto card1Content = std::make_unique<UIElement>();
//     card1Content->m_margin = {0, 0, 12, 0};
//     card1Content->m_background = {0.925, 0.988, 0.961, 1}; // #ecfdf5
//     //card1Content->m_border_radius = {8, 8, 8, 8};
//     card1Content->m_border_widths = {1, 1, 1, 1};
//     card1Content->m_border_color = {0.655, 0.953, 0.816, 1}; // #a7f3d0
//     card1Content->m_sizing.x_type = SizingType::Fixed;
//     card1Content->m_sizing.y_type = SizingType::Fixed;
//     card1Content->m_sizing.x_value = 188;
//     card1Content->m_sizing.y_value = 80;
//
//     auto card1Footer = std::make_unique<UIElement>();
//     card1Footer->m_background = {0.420, 0.447, 0.502, 1}; // #6b7280
//     card1Footer->m_border_radius = {6, 6, 6, 6};
//     card1Footer->m_border_widths = {1, 1, 1, 1};
//     card1Footer->m_border_color = {0.295, 0.337, 0.388, 1}; // #4b5563
//     card1Footer->m_sizing.x_type = SizingType::Fixed;
//     card1Footer->m_sizing.y_type = SizingType::Fixed;
//     card1Footer->m_sizing.x_value = 188;
//     card1Footer->m_sizing.y_value = 40;
//
//     auto card1 = std::make_unique<UIElement>();
//     card1->m_margin = {0, 8, 0, 0};
//     card1->m_padding = {20, 20, 20, 20};
//     card1->m_background = {1.0, 1.0, 1.0, 1};
//     card1->m_border_radius = {12, 12, 12, 12};
//     card1->m_border_widths = {2, 2, 2, 2};
//     card1->m_border_color = {0.898, 0.906, 0.922, 1}; // #e5e7eb
//     card1->m_shadow = {{0.0, 0.0, 0.0, 0.08}, {0, 4}, 16, 0};
//     card1->m_displayAxis = Axis::Vertical;
//     card1->AddChild(std::move(card1Header));
//     card1->AddChild(std::move(card1Content));
//     card1->AddChild(std::move(card1Footer));
//
//     // Card 2 (Red)
//     auto card2Header = std::make_unique<UIElement>();
//     card2Header->m_margin = {0, 0, 12, 0};
//     card2Header->m_background = {0.863, 0.149, 0.149, 1}; // #dc2626
//     card2Header->m_border_radius = {6, 6, 6, 6};
//     card2Header->m_border_widths = {2, 2, 2, 2};
//     card2Header->m_border_color = {0.725, 0.110, 0.110, 1}; // #b91c1c
//     card2Header->m_sizing.x_type = SizingType::Fixed;
//     card2Header->m_sizing.y_type = SizingType::Fixed;
//     card2Header->m_sizing.x_value = 188;
//     card2Header->m_sizing.y_value = 32;
//
//     auto card2Content = std::make_unique<UIElement>();
//     card2Content->m_margin = {0, 0, 12, 0};
//     card2Content->m_background = {0.996, 0.949, 0.949, 1}; // #fef2f2
//     card2Content->m_border_radius = {8, 8, 8, 8};
//     card2Content->m_border_widths = {1, 1, 1, 1};
//     card2Content->m_border_color = {0.988, 0.647, 0.647, 1}; // #fca5a5
//     card2Content->m_sizing.x_type = SizingType::Fixed;
//     card2Content->m_sizing.y_type = SizingType::Fixed;
//     card2Content->m_sizing.x_value = 188;
//     card2Content->m_sizing.y_value = 80;
//
//     auto card2Footer = std::make_unique<UIElement>();
//     card2Footer->m_background = {0.420, 0.447, 0.502, 1}; // #6b7280
//     card2Footer->m_border_radius = {6, 6, 6, 6};
//     card2Footer->m_border_widths = {1, 1, 1, 1};
//     card2Footer->m_border_color = {0.295, 0.337, 0.388, 1}; // #4b5563
//     card2Footer->m_sizing.x_type = SizingType::Fixed;
//     card2Footer->m_sizing.y_type = SizingType::Fixed;
//     card2Footer->m_sizing.x_value = 188;
//     card2Footer->m_sizing.y_value = 40;
//
//     auto card2 = std::make_unique<UIElement>();
//     card2->m_margin = {0, 8, 0, 8};
//     card2->m_padding = {20, 20, 20, 20};
//     card2->m_background = {1.0, 1.0, 1.0, 1};
//     card2->m_border_radius = {12, 12, 12, 12};
//     card2->m_border_widths = {2, 2, 2, 2};
//     card2->m_border_color = {0.898, 0.906, 0.922, 1}; // #e5e7eb
//     card2->m_shadow = {{0.0, 0.0, 0.0, 0.08}, {0, 4}, 16, 0};
//     card2->m_displayAxis = Axis::Vertical;
//     card2->AddChild(std::move(card2Header));
//     card2->AddChild(std::move(card2Content));
//     card2->AddChild(std::move(card2Footer));
//
//     // Card 3 (Purple)
//     auto card3Header = std::make_unique<UIElement>();
//     card3Header->m_margin = {0, 0, 12, 0};
//     card3Header->m_background = {0.486, 0.227, 0.929, 1}; // #7c3aed
//     card3Header->m_border_radius = {6, 6, 6, 6};
//     card3Header->m_border_widths = {2, 2, 2, 2};
//     card3Header->m_border_color = {0.427, 0.157, 0.851, 1}; // #6d28d9
//     card3Header->m_sizing.x_type = SizingType::Fixed;
//     card3Header->m_sizing.y_type = SizingType::Fixed;
//     card3Header->m_sizing.x_value = 188;
//     card3Header->m_sizing.y_value = 32;
//
//     auto card3Content = std::make_unique<UIElement>();
//     card3Content->m_margin = {0, 0, 12, 0};
//     card3Content->m_background = {0.961, 0.953, 1.0, 1}; // #f5f3ff
//     card3Content->m_border_radius = {8, 8, 8, 8};
//     card3Content->m_border_widths = {1, 1, 1, 1};
//     card3Content->m_border_color = {0.769, 0.710, 0.992, 1}; // #c4b5fd
//     card3Content->m_sizing.x_type = SizingType::Fixed;
//     card3Content->m_sizing.y_type = SizingType::Fixed;
//     card3Content->m_sizing.x_value = 188;
//     card3Content->m_sizing.y_value = 80;
//
//     auto card3Footer = std::make_unique<UIElement>();
//     card3Footer->m_background = {0.420, 0.447, 0.502, 1}; // #6b7280
//     card3Footer->m_border_radius = {6, 6, 6, 6};
//     card3Footer->m_border_widths = {1, 1, 1, 1};
//     card3Footer->m_border_color = {0.295, 0.337, 0.388, 1}; // #4b5563
//     card3Footer->m_sizing.x_type = SizingType::Fixed;
//     card3Footer->m_sizing.y_type = SizingType::Fixed;
//     card3Footer->m_sizing.x_value = 188;
//     card3Footer->m_sizing.y_value = 40;
//
//     auto card3 = std::make_unique<UIElement>();
//     card3->m_margin = {0, 0, 0, 8};
//     card3->m_padding = {20, 20, 20, 20};
//     card3->m_background = {1.0, 1.0, 1.0, 1};
//     card3->m_border_radius = {12, 12, 12, 12};
//     card3->m_border_widths = {2, 2, 2, 2};
//     card3->m_border_color = {0.898, 0.906, 0.922, 1}; // #e5e7eb
//     card3->m_shadow = {{0.0, 0.0, 0.0, 0.08}, {0, 4}, 16, 0};
//     card3->m_displayAxis = Axis::Vertical;
//     card3->AddChild(std::move(card3Header));
//     card3->AddChild(std::move(card3Content));
//     card3->AddChild(std::move(card3Footer));
//
//     // Card Grid Container
//     auto cardGrid = std::make_unique<UIElement>();
//     cardGrid->m_displayAxis = Axis::Horizontal;
//     cardGrid->m_sizing.x_type = SizingType::Fixed;
//     cardGrid->m_sizing.y_type = SizingType::Fixed;
//     cardGrid->m_sizing.x_value = 680;
//     cardGrid->m_sizing.y_value = 516; // Remaining height for cards
//     cardGrid->AddChild(std::move(card1));
//     cardGrid->AddChild(std::move(card2));
//     cardGrid->AddChild(std::move(card3));
//
//     // Content Area
//     auto content = std::make_unique<UIElement>();
//     content->m_padding = {20, 20, 20, 20};
//     content->m_background = {1.0, 1.0, 1.0, 1};
//     content->m_displayAxis = Axis::Vertical;
//     content->m_sizing.x_type = SizingType::Fixed;
//     content->m_sizing.y_type = SizingType::Fixed;
//     content->m_sizing.x_value = 720;
//     content->m_sizing.y_value = 580; // Total height - header
//     content->AddChild(std::move(contentHeader));
//     content->AddChild(std::move(cardGrid));
//
//     // Main Layout Container
//     auto mainLayout = std::make_unique<UIElement>();
//     mainLayout->m_displayAxis = Axis::Horizontal;
//     mainLayout->m_sizing.x_type = SizingType::Fixed;
//     mainLayout->m_sizing.y_type = SizingType::Fixed;
//     mainLayout->m_sizing.x_value = 960;
//     mainLayout->m_sizing.y_value = 580;
//     mainLayout->AddChild(std::move(sidebar));
//     mainLayout->AddChild(std::move(content));
//
//     // Root App Container
//     root = std::make_unique<UIElement>();
//     //root->m_margin = {20, 20, 20, 20};
//     root->m_background = {1.0, 1.0, 1.0, 1};
//     root->m_border_radius = {12, 12, 12, 12};
//     root->m_border_widths = {2, 2, 2, 2};
//     root->m_border_color = {0.878, 0.878, 0.878, 1}; // #e0e0e0
//     root->m_shadow = {{0.0, 0.0, 0.0, 0.12}, {0, 8}, 32, 0};
//     root->m_displayAxis = Axis::Vertical;
//     root->m_sizing.x_type = SizingType::Fixed;
//     root->m_sizing.y_type = SizingType::Fixed;
//     root->m_sizing.x_value = 960;
//     root->m_sizing.y_value = 640;
//     root->AddChild(std::move(header));
//     root->AddChild(std::move(mainLayout));
//
//
//     root->Scale(4);
// }

    Window::Window() {
        int wWidth = 1960;
        // windowFrame = std::make_unique<UIElement>();
        // windowFrame
        // ->HorizontalFixed(2400)
        // .VerticalFixed(1200);
        //windowFrame->AddChild(Tag::AutoMargin);

        root = std::make_unique<UIElement>();
        root->m_margin = {200, 0, 0, 0};
        root->m_padding = {-2, -2, 0, -2};
        root->m_background = {1.0, 1.0, 1.0, 1};
        root->m_border_radius = {12, 12, 12, 12};
        root->m_border_widths = {4, 14, 4, 4};
        root->m_border_color = {0.478, 0.478, 0.978, 0.8}; // #e0e0e0
        root->m_shadow = {{0.0, 0.0, 0.0, 0.10}, {0, 15}, 35, 0};
        root->m_displayAxis = Axis::Vertical;
        root->HorizontalFixed(1600);
        root->VerticalFixed(1600);

        //windowFrame->AddChild(std::move(root));
        //windowFrame->AddChild(Tag::AutoMargin);

        auto titleBar = std::make_unique<UIElement>();
        titleBar->m_margin = {0, 0, 0, 0};
        titleBar->m_padding = {17.5, 17.5, 17.5, 17.5};
        titleBar->m_background = {0.15, 0.2, 0.5, 1};
        titleBar->m_border_radius = {0, 12, 0, 12};
        //titleBar->m_shadow = {{0.0, 0.0, 0.0, 0.14}, {0, 4}, 50, 0};
        titleBar->m_displayAxis = Axis::Horizontal;
        titleBar->m_sizing.x_type = SizingType::Grow;
        titleBar->m_sizing.y_type = SizingType::Fixed;
        titleBar->m_sizing.x_value = wWidth + 20 + 2;
        titleBar->m_sizing.y_value = 60;


        auto ball1 = std::make_unique<UIElement>();
        ball1->m_margin = {0, 15, 15, 0};
        ball1->m_padding = {0, 0, 0, 0};
        ball1->m_background = {1, 0.4, 0.4, 1};
        ball1->m_border_radius = {12.5, 12.5, 12.5, 12.5};
        ball1->m_displayAxis = Axis::Vertical;
        ball1->m_sizing.x_type = SizingType::Fixed;
        ball1->m_sizing.y_type = SizingType::Fixed;
        ball1->m_sizing.x_value = 25;
        ball1->m_sizing.y_value = 25;
        titleBar->AddChild(std::move(ball1));

        auto ball2 = std::make_unique<UIElement>();
        ball2->m_margin = {0, 15, 15, 0};
        ball2->m_padding = {0, 0, 0, 0};
        ball2->m_background = {1, 0.7, 0.3, 1};
        ball2->m_border_radius = {12.5, 12.5, 12.5, 12.5};
        ball2->m_displayAxis = Axis::Vertical;
        ball2->m_sizing.x_type = SizingType::Fixed;
        ball2->m_sizing.y_type = SizingType::Fixed;
        ball2->m_sizing.x_value = 25;
        ball2->m_sizing.y_value = 25;
        titleBar->AddChild(std::move(ball2));
        auto ball3 = std::make_unique<UIElement>();
        ball3->m_margin = {0, 15, 15, 0};
        ball3->m_padding = {0, 0, 0, 0};
        ball3->m_background = {0.4, 0.8, 0.4, 1};
        ball3->m_border_radius = {12.5, 12.5, 12.5, 12.5};
        ball3->m_displayAxis = Axis::Vertical;
        ball3->m_sizing.x_type = SizingType::Fixed;
        ball3->m_sizing.y_type = SizingType::Fixed;
        ball3->m_sizing.x_value = 25;
        ball3->m_sizing.y_value = 25;
        titleBar->AddChild(std::move(ball3));

        root->AddChild(std::move(titleBar));
        // Tunables for your perf test
        const int kRows     = 30;   // how many top-level rows
        const int kPerRow   = 50;   // how many items per row
        const int kRowW     = 400;  // row width  (px)
        const int kRowH     = 34;   // row height (px) — a bit taller than item height + vertical margins
        const int kItemW    = 24;   // item width
        const int kItemH    = 24;   // item height

        //root->AddChild(Tag::AutoMargin);
        auto windowFrame = std::make_unique<UIElement>();
        windowFrame->HorizontalGrow();
        windowFrame->VerticalGrow();
        windowFrame->Background({0.90f, 0.30f, 0.0f, 0.2f});
        windowFrame->MarginBottom(1);
        windowFrame->HorizontalStack();
        windowFrame->m_border_radius = {0, 0, 12, 12};
        //windowFrame->InvisibleButOccupySpace(true);


        auto container = std::make_unique<UIElement>();
        container->HorizontalFixed(800);
        container->VerticalGrow();
        container->VerticalStack();
        container->Background({0.90f, 0.90f, 0.90f, 1.0f});
        container->m_border_widths = {2, 20, 2, 2};
        container->m_border_color = {0.0, 0.906, 0.922, 1}; // #e5e7eb
        //container->InvisibleButOccupySpace(true);



        std::vector<std::string> text = {
            "This is line 1.",
            "Music.mp3",
            "If you want to switch to it, basically in the shader all you need to change is the in vec4 colors; to uniform vec4 colors; ",
            "Then delete the portion in mesh render for the AttrColors, no idea if you can use it for the separate rgba vec4,  but its easier to just send it as a uniform similar to the checkbox example",
            "also very important, don't forget it",
        };

        for (int i = 0; i < text.size(); ++i) {
            auto row = std::make_unique<UIElement>(Tag::Paragraph);
            row->m_background = {0.90f, 0.90f, 0.90f, 1.0f};
            row->m_margin     = {5, 10, 5, 10}; // L T R B
            row->m_padding    = {0, 0, 5, 0}; // L T R B
            row->m_border_widths  = {2, 0, 2, 2};
            row->m_border_color   = {0.478f, 0.478f, 0.778f, 1.0f};
            row->TextColor({0,0,0,1.0f}).Text(text[i]).VerticalGrow().HorizontalGrow();
            row->m_sizing.x_type  = SizingType::Grow;
            row->VerticalFixed(200);


            container->AddChild(std::move(row));
        }
        windowFrame->AddChild(std::move(container));
        windowFrame->AddChild(Tag::AutoMargin);

        root->AddChild(std::move(windowFrame));

    }



    auto Window::Render(Graphic::GL::UIRenderer& renderer, Graphic::GL::TextRenderer& text_render, float target_x, float target_y, const glm::vec2& screenSize) -> void {
        root->CalculateFitSizeOnAxis(Axis::Horizontal);
        root->CalculateGrowSizeOnAxis(Axis::Horizontal);

        root->CalculateFitSizeOnAxis(Axis::Vertical);
        root->CalculateGrowSizeOnAxis(Axis::Vertical);

        root->PositionOnAxis(Axis::Horizontal, 0);
        root->PositionOnAxis(Axis::Vertical, 0);
        root->RenderQueue(renderer,text_render, target_x, target_y);

        root->MarginLeft((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
        root->MarginRight((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);

    }

}
