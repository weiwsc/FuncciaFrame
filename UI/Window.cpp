//
// Created by Wangsicong Wei on 2025-09-26.
//

#include "Window.h"
#include "UIElement.h"
#include <chrono>
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



    Window::Window() {

        TextUI();
    }





    auto Window::TextUI() -> void {
        root = std::make_unique<UIElement>();
        root->Padding(20);
        root->Background({0.12f, 0.12f, 0.12f, 0.0f});
        root->VerticalStack();
        root->HorizontalFixed(2000);
        root->VerticalFixed(1000);

        auto textbox = std::make_unique<UIElement>();
        textbox->Padding(0);
        textbox->Background({1, 1, 1, 1.0f});
        textbox->BorderWidth({3, 3, 3, 3});
        textbox->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});
        textbox->VerticalStack();
        textbox->HorizontalGrow();
        textbox->VerticalFixed(200);
        textbox->Text("Hello World!");

        root->AddChild(std::move(textbox));
    }



    auto Window::Render(Graphic::GL::UIRenderer& renderer, Graphic::GL::TextRenderer& text_renderer, float target_x, float target_y, const glm::vec2& screenSize) -> void {
        root->CalculateFitSizeOnAxis(Axis::Horizontal);
        root->CalculateGrowSizeOnAxis(Axis::Horizontal);

        root->CalculateFitSizeOnAxis(Axis::Vertical);
        root->CalculateGrowSizeOnAxis(Axis::Vertical);

        root->PositionOnAxis(Axis::Horizontal, 0);
        root->PositionOnAxis(Axis::Vertical, 0);



        root->GlobalPositionPass(target_x, target_y);
        root->CullingPass(vec4(0 , 0, screenSize.x, screenSize.y));
        root->RenderQueue(renderer,text_renderer, target_x, target_y);

        root->MarginLeft((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
        root->MarginRight((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);

    }

    auto Window::RenderProfile(Graphic::GL::UIRenderer& renderer,Graphic::GL::TextRenderer& text_renderer,  Graphic::GL::TextRenderer& text_render,
                    float target_x, float target_y, const glm::vec2& screenSize) -> void {

    auto start = std::chrono::high_resolution_clock::now();
    root->CalculateFitSizeOnAxis(Axis::Horizontal);
    auto end = std::chrono::high_resolution_clock::now();
    double fitX = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    root->CalculateGrowSizeOnAxis(Axis::Horizontal);
    end = std::chrono::high_resolution_clock::now();
    double growX = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    root->CalculateFitSizeOnAxis(Axis::Vertical);
    end = std::chrono::high_resolution_clock::now();
    double fitY = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    root->CalculateGrowSizeOnAxis(Axis::Vertical);
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

    auto Window::InitLayout(float target_x, float target_y, const glm::vec2& screenSize) -> void {
        //root->Scale(2);
        root->GlobalPositionPass(target_x, target_y);
        root->CullingPass(vec4(0 , 0, screenSize.x, screenSize.y));
        //root->MarginLeft((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
        //root->MarginRight((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
        root->CalculateFitSizeOnAxis(Axis::Horizontal);
        root->CalculateGrowSizeOnAxis(Axis::Horizontal);

        root->CalculateFitSizeOnAxis(Axis::Vertical);
        root->CalculateGrowSizeOnAxis(Axis::Vertical);

        root->PositionOnAxis(Axis::Horizontal, 0);
        root->PositionOnAxis(Axis::Vertical, 0);
#ifdef FF_UI_LAZY_LAYOUT
        root->FlipLazyLayoutPass();
#endif
    }

    auto Window::UI1() -> void {
        root = std::make_unique<UIElement>();
root->Margin(200, 0, 0, 0);
root->Padding(0);
root->Background({0.95f, 0.95f, 0.95f, 1.0f});
root->BorderRadius({12, 12, 12, 12});
root->BorderWidth({1, 1, 1, 1});
root->BorderColor({0.8f, 0.8f, 0.8f, 1.0f});
root->BoxShadow({0, 10}, 40, 0, {0.0f, 0.0f, 0.0f, 0.15f});
root->VerticalStack();
root->HorizontalFixed(1400);
root->VerticalFixed(900);

// Title Bar with traffic lights
auto titleBar = std::make_unique<UIElement>();
titleBar->Padding(12, 20, 12, 12);
titleBar->Background({0.95f, 0.95f, 0.95f, 1.0f});
titleBar->BorderRadius({12, 12, 12, 12});
titleBar->HorizontalStack();
titleBar->HorizontalGrow();
titleBar->VerticalFixed(52);

// Traffic lights container
auto trafficLights = std::make_unique<UIElement>();
//trafficLights->BorderRadius({12, 12, 0, 0});
trafficLights->HorizontalStack();
trafficLights->HorizontalFixed(79);
trafficLights->VerticalGrow();

// Red button
auto redButton = std::make_unique<UIElement>();
redButton->Margin(0, 8, 0, 0);
redButton->Background({1.0f, 0.38f, 0.38f, 1.0f});
redButton->BorderRadius({9, 9, 9, 9});
redButton->HorizontalFixed(18);
redButton->VerticalFixed(18);
trafficLights->AddChild(std::move(redButton));

// Yellow button
auto yellowButton = std::make_unique<UIElement>();
yellowButton->Margin(0, 8, 0, 0);
yellowButton->Background({1.0f, 0.76f, 0.28f, 1.0f});
yellowButton->BorderRadius({9, 9, 9, 9});
yellowButton->HorizontalFixed(18);
yellowButton->VerticalFixed(18);
trafficLights->AddChild(std::move(yellowButton));

// Green button
auto greenButton = std::make_unique<UIElement>();
greenButton->Background({0.27f, 0.84f, 0.35f, 1.0f});
greenButton->BorderRadius({9, 9, 9, 9});
greenButton->HorizontalFixed(18);
greenButton->VerticalFixed(18);
trafficLights->AddChild(std::move(greenButton));

titleBar->AddChild(std::move(trafficLights));

// Window title (center)
auto titleSpace = std::make_unique<UIElement>();
titleSpace->VerticalGrow();
titleBar->AddChild(std::move(titleSpace));

// Right side spacer to balance
auto rightSpacer = std::make_unique<UIElement>();
rightSpacer->HorizontalFixed(68);
rightSpacer->VerticalGrow();
titleBar->AddChild(std::move(rightSpacer));

root->AddChild(std::move(titleBar));

// Toolbar
auto toolbar = std::make_unique<UIElement>();
toolbar->Padding(8, 12, 8, 12);
toolbar->Background({0.98f, 0.98f, 0.98f, 1.0f});
toolbar->BorderWidth({0, 0, 1, 0});
toolbar->BorderColor({0.85f, 0.85f, 0.85f, 1.0f});
toolbar->HorizontalStack();
toolbar->HorizontalGrow();
toolbar->VerticalFixed(44);

// Toolbar buttons
for (int i = 0; i < 5; i++) {
    auto button = std::make_unique<UIElement>();
    button->Margin(0, 6, 0, 0);
    button->Background({0.94f, 0.94f, 0.94f, 1.0f});
    button->BorderRadius({4, 4, 4, 4});
    button->BorderWidth({1, 1, 1, 1});
    button->BorderColor({0.82f, 0.82f, 0.82f, 1.0f});
    button->HorizontalFixed(32);
    button->VerticalFixed(28);
    toolbar->AddChild(std::move(button));
}

root->AddChild(std::move(toolbar));

// Main content area
auto mainArea = std::make_unique<UIElement>();
mainArea->HorizontalStack();
mainArea->HorizontalGrow();
mainArea->VerticalGrow();

// Sidebar
auto sidebar = std::make_unique<UIElement>();
sidebar->Background({0.96f, 0.96f, 0.96f, 1.0f});
sidebar->BorderWidth({0, 1, 0, 0});
sidebar->BorderColor({0.85f, 0.85f, 0.85f, 1.0f});
sidebar->VerticalStack();
sidebar->HorizontalFixed(220);
sidebar->VerticalGrow();

// Sidebar header
auto sidebarHeader = std::make_unique<UIElement>();
sidebarHeader->Padding(12, 8, 12, 8);
sidebarHeader->Background({0.94f, 0.94f, 0.94f, 1.0f});
sidebarHeader->BorderWidth({0, 0, 1, 0});
sidebarHeader->BorderColor({0.85f, 0.85f, 0.85f, 1.0f});
sidebarHeader->HorizontalGrow();
sidebarHeader->VerticalFixed(36);
sidebar->AddChild(std::move(sidebarHeader));

// Sidebar items
for (int i = 0; i < 20; i++) {
    auto item = std::make_unique<UIElement>();
    item->Padding(8, 6, 8, 6);

    // Selected item
    if (i == 3) {
        item->Background({0.0f, 0.48f, 1.0f, 0.12f});
    } else {
        item->Background({0.96f, 0.96f, 0.96f, 1.0f});
    }

    item->HorizontalGrow();
    item->VerticalFixed(24);

    // Indent nested items
    if (i % 5 == 1 || i % 5 == 2) {
        item->MarginLeft(16);
    }

    sidebar->AddChild(std::move(item));
}

mainArea->AddChild(std::move(sidebar));

// Editor container
auto editorContainer = std::make_unique<UIElement>();
editorContainer->VerticalStack();
editorContainer->HorizontalGrow();
editorContainer->VerticalGrow();

// Tab bar
auto tabBar = std::make_unique<UIElement>();
tabBar->Background({0.97f, 0.97f, 0.97f, 1.0f});
tabBar->BorderWidth({0, 0, 1, 0});
tabBar->BorderColor({0.85f, 0.85f, 0.85f, 1.0f});
tabBar->HorizontalStack();
tabBar->HorizontalGrow();
tabBar->VerticalFixed(32);

// Tabs
for (int i = 0; i < 4; i++) {
    auto tab = std::make_unique<UIElement>();
    tab->Padding(8, 6, 8, 6);
    tab->BorderWidth({0, 1, 0, 0});
    tab->BorderColor({0.85f, 0.85f, 0.85f, 1.0f});

    if (i == 0) {
        // Active tab
        tab->Background({1.0f, 1.0f, 1.0f, 1.0f});
    } else {
        tab->Background({0.97f, 0.97f, 0.97f, 1.0f});
    }

    tab->HorizontalFixed(140);
    tab->VerticalGrow();

    tabBar->AddChild(std::move(tab));
}

editorContainer->AddChild(std::move(tabBar));

// Editor area
auto editorArea = std::make_unique<UIElement>();
editorArea->Background({1.0f, 1.0f, 1.0f, 1.0f});
editorArea->HorizontalStack();
editorArea->HorizontalGrow();
editorArea->VerticalGrow();

// Line numbers
auto lineNumbers = std::make_unique<UIElement>();
lineNumbers->Padding(8, 4, 8, 4);
lineNumbers->Background({0.97f, 0.97f, 0.97f, 1.0f});
lineNumbers->BorderWidth({0, 1, 0, 0});
lineNumbers->BorderColor({0.90f, 0.90f, 0.90f, 1.0f});
lineNumbers->VerticalStack();
lineNumbers->HorizontalFixed(48);
lineNumbers->VerticalGrow();

for (int i = 1; i <= 35; i++) {
    auto lineNum = std::make_unique<UIElement>();
    lineNum->Background({0.97f, 0.97f, 0.97f, 1.0f});
    lineNum->HorizontalGrow();
    lineNum->VerticalFixed(20);
    lineNumbers->AddChild(std::move(lineNum));
}

editorArea->AddChild(std::move(lineNumbers));

// Code content
auto codeContent = std::make_unique<UIElement>();
codeContent->Padding(8, 4, 8, 4);
codeContent->Background({1.0f, 1.0f, 1.0f, 1.0f});
codeContent->VerticalStack();
codeContent->HorizontalGrow();
codeContent->VerticalGrow();

for (int i = 0; i < 35; i++) {
    auto codeLine = std::make_unique<UIElement>();
    codeLine->Background({1.0f, 1.0f, 1.0f, 1.0f});

    // Current line highlight
    if (i == 8) {
        codeLine->Background({0.0f, 0.48f, 1.0f, 0.08f});
    }

    codeLine->HorizontalGrow();
    codeLine->VerticalFixed(20);
    codeContent->AddChild(std::move(codeLine));
}

editorArea->AddChild(std::move(codeContent));
editorContainer->AddChild(std::move(editorArea));

mainArea->AddChild(std::move(editorContainer));

// Inspector panel
auto inspector = std::make_unique<UIElement>();
inspector->Background({0.96f, 0.96f, 0.96f, 1.0f});
inspector->BorderWidth({0, 0, 0, 1});
inspector->BorderColor({0.85f, 0.85f, 0.85f, 1.0f});
inspector->VerticalStack();
inspector->HorizontalFixed(240);
inspector->VerticalGrow();

// Inspector header
auto inspectorHeader = std::make_unique<UIElement>();
inspectorHeader->Padding(12, 8, 12, 8);
inspectorHeader->Background({0.94f, 0.94f, 0.94f, 1.0f});
inspectorHeader->BorderWidth({0, 0, 1, 0});
inspectorHeader->BorderColor({0.85f, 0.85f, 0.85f, 1.0f});
inspectorHeader->HorizontalGrow();
inspectorHeader->VerticalFixed(36);
inspector->AddChild(std::move(inspectorHeader));

// Inspector sections
for (int section = 0; section < 3; section++) {
    // Section header
    auto sectionHeader = std::make_unique<UIElement>();
    sectionHeader->Padding(8, 6, 8, 6);
    sectionHeader->Background({0.92f, 0.92f, 0.92f, 1.0f});
    sectionHeader->BorderWidth({0, 0, 1, 0});
    sectionHeader->BorderColor({0.85f, 0.85f, 0.85f, 1.0f});
    sectionHeader->HorizontalGrow();
    sectionHeader->VerticalFixed(28);
    inspector->AddChild(std::move(sectionHeader));

    // Section items
    for (int i = 0; i < 5; i++) {
        auto item = std::make_unique<UIElement>();
        item->Padding(12, 6, 12, 6);
        item->Background({0.96f, 0.96f, 0.96f, 1.0f});
        item->HorizontalGrow();
        item->VerticalFixed(28);
        inspector->AddChild(std::move(item));
    }
}

mainArea->AddChild(std::move(inspector));
root->AddChild(std::move(mainArea));

// Bottom panel (optional debug console)
auto bottomPanel = std::make_unique<UIElement>();
bottomPanel->Background({0.97f, 0.97f, 0.97f, 1.0f});
bottomPanel->BorderWidth({1, 0, 0, 0});
bottomPanel->BorderColor({0.85f, 0.85f, 0.85f, 1.0f});
bottomPanel->BorderRadius({12, 12, 12, 12});
bottomPanel->VerticalStack();
bottomPanel->HorizontalGrow();
bottomPanel->VerticalFixed(140);

// Console header
auto consoleHeader = std::make_unique<UIElement>();
consoleHeader->Padding(8, 6, 8, 6);
consoleHeader->Background({0.94f, 0.94f, 0.94f, 1.0f});
consoleHeader->BorderWidth({0, 0, 1, 0});
consoleHeader->BorderColor({0.85f, 0.85f, 0.85f, 1.0f});
consoleHeader->HorizontalGrow();
consoleHeader->VerticalFixed(28);
bottomPanel->AddChild(std::move(consoleHeader));

// Console content
auto consoleContent = std::make_unique<UIElement>();
consoleContent->Padding(8);
consoleContent->Background({0.99f, 0.99f, 0.99f, 1.0f});
consoleContent->VerticalStack().BorderRadius({12, 12, 12, 12});
consoleContent->HorizontalGrow();
consoleContent->VerticalGrow();

for (int i = 0; i < 6; i++) {
    auto consoleLine = std::make_unique<UIElement>();
    consoleLine->Background({0.99f, 0.99f, 0.99f, 1.0f});
    consoleLine->HorizontalGrow();
    consoleLine->VerticalFixed(16);
    consoleContent->AddChild(std::move(consoleLine));
}

bottomPanel->AddChild(std::move(consoleContent));
root->AddChild(std::move(bottomPanel));
    }

    auto Window::UI2() -> void {
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
        container->VerticalFixed(800);
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

        for (int i = 0; i < 13; ++i) {
            auto row = std::make_unique<UIElement>(Tag::Paragraph);
            row->m_background = {0.90f, 0.90f, 0.90f, 1.0f};
            row->m_margin     = {5, 10, 5, 10}; // L T R B
            row->m_padding    = {0, 0, 5, 0}; // L T R B
            row->m_border_widths  = {2, 0, 2, 2};
            row->m_border_color   = {0.478f, 0.478f, 0.778f, 1.0f};
            //row->TextColor({0,0,0,1.0f}).Text(text[i]).VerticalGrow().HorizontalGrow();
            row->m_sizing.x_type  = SizingType::Grow;
            row->VerticalFixed(200);


            container->AddChild(std::move(row));
        }
        windowFrame->AddChild(std::move(container));
        windowFrame->AddChild(Tag::AutoMargin);

        root->AddChild(std::move(windowFrame));
    }

    auto Window::UI3() -> void {
    root = std::make_unique<UIElement>();
    root->Padding(0);
        root->Margin(0,40);
    root->Background({0.11f, 0.11f, 0.12f, 1.0f}); // Dark theme
    root->BorderRadius({16, 16, 16, 16});
    root->BoxShadow({0, 20}, 60, -10, {0.0f, 0.0f, 0.0f, 0.4f});
    root->VerticalStack();
    root->HorizontalGrow(); // HiDPI friendly
    root->VerticalFixed(1600);

    // ========== TITLE BAR ==========
    auto titleBar = std::make_unique<UIElement>();
    titleBar->Padding(16, 24, 16, 24);
    titleBar->Background({0.14f, 0.14f, 0.15f, 1.0f});
    titleBar->BorderRadius({16, 16, 16, 16});
    titleBar->HorizontalStack();
    titleBar->HorizontalGrow();
    titleBar->VerticalFixed(68);

    // Traffic lights
    auto trafficLights = std::make_unique<UIElement>();
    trafficLights->HorizontalStack().MarginRight(20);
    trafficLights->HorizontalFixed(100);
    trafficLights->VerticalGrow();

    auto redBtn = std::make_unique<UIElement>();
    redBtn->Margin(0, 12, 0, 0);
    redBtn->Background({1.0f, 0.36f, 0.36f, 1.0f});
    redBtn->BorderRadius({12, 12, 12, 12});
    redBtn->HorizontalFixed(24);
    redBtn->VerticalFixed(24);
    trafficLights->AddChild(std::move(redBtn));

    auto yellowBtn = std::make_unique<UIElement>();
    yellowBtn->Margin(0, 12, 0, 0);
    yellowBtn->Background({1.0f, 0.74f, 0.26f, 1.0f});
    yellowBtn->BorderRadius({12, 12, 12, 12});
    yellowBtn->HorizontalFixed(24);
    yellowBtn->VerticalFixed(24);
    trafficLights->AddChild(std::move(yellowBtn));

    auto greenBtn = std::make_unique<UIElement>();
    greenBtn->Background({0.25f, 0.82f, 0.33f, 1.0f});
    greenBtn->BorderRadius({12, 12, 12, 12});
    greenBtn->HorizontalFixed(24);
    greenBtn->VerticalFixed(24);
    trafficLights->AddChild(std::move(greenBtn));

    titleBar->AddChild(std::move(trafficLights));

    // Title text placeholder
    auto titleText = std::make_unique<UIElement>();
    titleText->Margin(0, 20, 0, 0);
    titleText->Background({0.3f, 0.3f, 0.32f, 1.0f});
    titleText->BorderRadius({6, 6, 6, 6});
    titleText->HorizontalFixed(280);
    titleText->VerticalFixed(32);
    titleBar->AddChild(std::move(titleText));

    auto spacer1 = std::make_unique<UIElement>();
    spacer1->HorizontalGrow();
    titleBar->AddChild(std::move(spacer1));

    // Search bar
    auto searchBar = std::make_unique<UIElement>();
    searchBar->Padding(12, 16, 12, 16);
    searchBar->Background({0.18f, 0.18f, 0.19f, 1.0f});
    searchBar->BorderRadius({8, 8, 8, 8});
    searchBar->BorderWidth({2, 2, 2, 2});
    searchBar->BorderColor({0.25f, 0.25f, 0.27f, 1.0f});
    searchBar->HorizontalFixed(400);
    searchBar->VerticalGrow();
    titleBar->AddChild(std::move(searchBar));

    auto spacer2 = std::make_unique<UIElement>();
    spacer2->HorizontalGrow();
    titleBar->AddChild(std::move(spacer2));

    // User avatar placeholder
    auto avatar = std::make_unique<UIElement>();
    avatar->Background({0.4f, 0.5f, 0.9f, 1.0f});
    avatar->BorderRadius({20, 20, 20, 20});
    avatar->BorderWidth({2, 2, 2, 2});
    avatar->BorderColor({0.5f, 0.6f, 1.0f, 0.3f});
    avatar->HorizontalFixed(40);
    avatar->VerticalFixed(40);
    titleBar->AddChild(std::move(avatar));

    root->AddChild(std::move(titleBar));

    // ========== TOOLBAR ==========
    auto toolbar = std::make_unique<UIElement>();
    toolbar->Padding(12, 20, 12, 20);
    toolbar->Background({0.13f, 0.13f, 0.14f, 1.0f});
    toolbar->BorderWidth({0, 0, 1, 0});
    toolbar->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});
    toolbar->HorizontalStack();
    toolbar->HorizontalGrow();
    toolbar->VerticalFixed(72);

    // Icon buttons with more visual interest
    for (int i = 0; i < 8; i++) {
        auto btnGroup = std::make_unique<UIElement>();
        btnGroup->Margin(0, 8, 0, 0);
        btnGroup->Padding(12);
        btnGroup->Background({0.18f, 0.18f, 0.19f, 1.0f});
        btnGroup->BorderRadius({8, 8, 8, 8});
        btnGroup->BorderWidth({1, 1, 1, 1});
        btnGroup->BorderColor({0.25f, 0.25f, 0.27f, 1.0f});

        // Highlight active button
        if (i == 2) {
            btnGroup->Background({0.3f, 0.4f, 0.9f, 0.15f});
            btnGroup->BorderColor({0.4f, 0.5f, 1.0f, 0.5f});
        }

        btnGroup->HorizontalFixed(48);
        btnGroup->VerticalFixed(48);
        toolbar->AddChild(std::move(btnGroup));

        if (i == 3) { // Add separator
            auto sep = std::make_unique<UIElement>();
            sep->Margin(0, 12, 0, 8);
            sep->Background({0.2f, 0.2f, 0.22f, 1.0f});
            sep->HorizontalFixed(2);
            sep->VerticalGrow();
            toolbar->AddChild(std::move(sep));
        }
    }

    auto toolbarSpacer = std::make_unique<UIElement>();
    toolbarSpacer->HorizontalGrow();
    toolbar->AddChild(std::move(toolbarSpacer));

    // Branch indicator
    auto branchIndicator = std::make_unique<UIElement>();
    branchIndicator->Padding(10, 16, 10, 16);
    branchIndicator->Background({0.2f, 0.7f, 0.4f, 0.15f});
    branchIndicator->BorderRadius({6, 6, 6, 6});
    branchIndicator->BorderWidth({1, 1, 1, 1});
    branchIndicator->BorderColor({0.2f, 0.7f, 0.4f, 0.4f});
    branchIndicator->HorizontalFixed(140);
    branchIndicator->VerticalGrow();
    toolbar->AddChild(std::move(branchIndicator));

    root->AddChild(std::move(toolbar));

    // ========== MAIN CONTENT ==========
    auto mainArea = std::make_unique<UIElement>();
    mainArea->HorizontalStack();
    mainArea->HorizontalGrow();
    mainArea->VerticalGrow();

    // ========== SIDEBAR (File Tree) ==========
    auto sidebar = std::make_unique<UIElement>();
    sidebar->Background({0.12f, 0.12f, 0.13f, 1.0f});
    sidebar->BorderWidth({0, 1, 0, 0});
    sidebar->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});
    sidebar->VerticalStack();
    sidebar->HorizontalFixed(320);
    sidebar->VerticalGrow();

    // Sidebar header
    auto sidebarHeader = std::make_unique<UIElement>();
    sidebarHeader->Padding(16, 12, 16, 12);
    sidebarHeader->Background({0.14f, 0.14f, 0.15f, 1.0f});
    sidebarHeader->BorderWidth({0, 0, 1, 0});
    sidebarHeader->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});
    sidebarHeader->HorizontalStack();
    sidebarHeader->HorizontalGrow();
    sidebarHeader->VerticalFixed(56);

    auto sidebarTitle = std::make_unique<UIElement>();
    sidebarTitle->Background({0.25f, 0.25f, 0.27f, 1.0f});
    sidebarTitle->BorderRadius({4, 4, 4, 4});
    sidebarTitle->HorizontalFixed(100);
    sidebarTitle->VerticalFixed(28);
    sidebarHeader->AddChild(std::move(sidebarTitle));

    sidebar->AddChild(std::move(sidebarHeader));

    // File tree items with proper indentation
    for (int i = 0; i < 30; i++) {
        auto item = std::make_unique<UIElement>();
        item->Padding(12, 10, 12, 10);
        item->HorizontalStack();

        int depth = 0;
        if (i % 8 == 1 || i % 8 == 2) depth = 1;
        if (i % 8 == 5 || i % 8 == 6) depth = 2;

        item->MarginLeft(depth * 24.0f);

        // Selected/hovered states
        if (i == 7) {
            item->Background({0.3f, 0.4f, 0.9f, 0.2f});
            item->BorderWidth({0, 0, 0, 3});
            item->BorderColor({0.4f, 0.5f, 1.0f, 1.0f});
        } else if (i == 12) {
            item->Background({0.18f, 0.18f, 0.19f, 1.0f});
        }

        item->HorizontalGrow();
        item->VerticalFixed(36);

        // File icon placeholder
        auto icon = std::make_unique<UIElement>();
        icon->Margin(0, 12, 0, 0);

        // Different colors for folders vs files
        if (i % 8 == 0 || i % 8 == 3) {
            icon->Background({0.9f, 0.7f, 0.3f, 1.0f}); // Folder
        } else {
            icon->Background({0.4f, 0.6f, 0.9f, 1.0f}); // File
        }

        icon->BorderRadius({4, 4, 4, 4});
        icon->HorizontalFixed(20);
        icon->VerticalFixed(20);
        item->AddChild(std::move(icon));

        // Filename placeholder
        auto filename = std::make_unique<UIElement>();
        filename->Background({0.22f, 0.22f, 0.24f, 1.0f});
        filename->BorderRadius({3, 3, 3, 3});
        filename->HorizontalFixed(80 + (i % 5) * 20);
        filename->VerticalFixed(20);
        item->AddChild(std::move(filename));

        sidebar->AddChild(std::move(item));
    }

    mainArea->AddChild(std::move(sidebar));

    // ========== EDITOR AREA ==========
    auto editorContainer = std::make_unique<UIElement>();
    editorContainer->VerticalStack();
    editorContainer->HorizontalGrow();
    editorContainer->VerticalGrow();

    // Tab bar with better styling
    auto tabBar = std::make_unique<UIElement>();
    tabBar->Background({0.13f, 0.13f, 0.14f, 1.0f});
    tabBar->BorderWidth({0, 0, 1, 0});
    tabBar->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});
    tabBar->HorizontalStack();
    tabBar->HorizontalGrow();
    tabBar->VerticalFixed(52);

    for (int i = 0; i < 5; i++) {
        auto tab = std::make_unique<UIElement>();
        tab->Padding(14, 20, 14, 20);
        tab->HorizontalStack();
        tab->BorderWidth({0, 1, 0, 0});
        tab->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});

        if (i == 1) {
            // Active tab
            tab->Background({0.16f, 0.16f, 0.17f, 1.0f});
            tab->BorderWidth({0, 1, 3, 0});
            tab->BorderColor({0.4f, 0.5f, 1.0f, 1.0f});
        } else {
            tab->Background({0.13f, 0.13f, 0.14f, 1.0f});
        }

        tab->HorizontalFixed(200);
        tab->VerticalGrow();

        // Tab icon
        auto tabIcon = std::make_unique<UIElement>();
        tabIcon->Margin(0, 12, 0, 0);
        tabIcon->Background({0.4f, 0.6f, 0.9f, 1.0f});
        tabIcon->BorderRadius({3, 3, 3, 3});
        tabIcon->HorizontalFixed(16);
        tabIcon->VerticalFixed(16);
        tab->AddChild(std::move(tabIcon));

        // Tab title
        auto tabTitle = std::make_unique<UIElement>();
        tabTitle->Background({0.25f, 0.25f, 0.27f, 1.0f});
        tabTitle->BorderRadius({3, 3, 3, 3});
        tabTitle->HorizontalGrow();
        tabTitle->VerticalFixed(18);
        tab->AddChild(std::move(tabTitle));

        tabBar->AddChild(std::move(tab));
    }

    editorContainer->AddChild(std::move(tabBar));

    // Editor with minimap
    auto editorArea = std::make_unique<UIElement>();
    editorArea->Background({0.15f, 0.15f, 0.16f, 1.0f});
    editorArea->HorizontalStack();
    editorArea->HorizontalGrow();
    editorArea->VerticalGrow();

    // Line numbers with better contrast
    auto lineNumbers = std::make_unique<UIElement>();
    lineNumbers->Padding(12, 16, 12, 16);
    lineNumbers->Background({0.13f, 0.13f, 0.14f, 1.0f});
    lineNumbers->BorderWidth({0, 1, 0, 0});
    lineNumbers->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});
    lineNumbers->VerticalStack();
    lineNumbers->HorizontalFixed(72);
    lineNumbers->VerticalGrow();

    for (int i = 1; i <= 40; i++) {
        auto lineNum = std::make_unique<UIElement>();
        lineNum->Padding(4, 0, 4, 0);
        lineNum->Background({0.2f, 0.2f, 0.22f, 1.0f});
        lineNum->BorderRadius({3, 3, 3, 3});

        // Highlight current line
        if (i == 12) {
            lineNum->Background({0.3f, 0.4f, 0.9f, 0.2f});
        }

        lineNum->HorizontalGrow();
        lineNum->VerticalFixed(28);
        lineNumbers->AddChild(std::move(lineNum));
    }

    editorArea->AddChild(std::move(lineNumbers));

    // Code content with syntax highlighting placeholders
    auto codeContent = std::make_unique<UIElement>();
    codeContent->Padding(16, 20, 16, 20);
    codeContent->Background({0.15f, 0.15f, 0.16f, 1.0f});
    codeContent->VerticalStack();
    codeContent->HorizontalGrow();
    codeContent->VerticalGrow();

    for (int i = 0; i < 40; i++) {
        auto codeLine = std::make_unique<UIElement>();
        codeLine->Padding(6, 0, 6, 0);
        codeLine->HorizontalStack();

        // Current line highlight
        if (i == 11) {
            codeLine->Background({0.3f, 0.4f, 0.9f, 0.08f});
            codeLine->BorderRadius({4, 4, 4, 4});
        }

        codeLine->HorizontalGrow();
        codeLine->VerticalFixed(28);

        // Code tokens with different colors (syntax highlighting placeholders)
        if (i % 5 != 0 && i < 35) {
            int numTokens = 2 + (i % 4);
            for (int t = 0; t < numTokens; t++) {
                auto token = std::make_unique<UIElement>();
                token->Margin(0, 12, 0, 0);
                token->BorderRadius({2, 2, 2, 2});

                // Different colors for different token types
                if (t == 0) {
                    token->Background({0.8f, 0.4f, 0.9f, 1.0f}); // Keyword
                } else if (t % 3 == 1) {
                    token->Background({0.4f, 0.8f, 0.6f, 1.0f}); // String
                } else if (t % 3 == 2) {
                    token->Background({0.9f, 0.7f, 0.4f, 1.0f}); // Number
                } else {
                    token->Background({0.6f, 0.7f, 0.9f, 1.0f}); // Function
                }

                token->HorizontalFixed(40 + (t * i % 5) * 15);
                token->VerticalFixed(16);
                codeLine->AddChild(std::move(token));
            }
        }

        codeContent->AddChild(std::move(codeLine));
    }

    editorArea->AddChild(std::move(codeContent));

    // Minimap
    auto minimap = std::make_unique<UIElement>();
    minimap->Padding(8);
    minimap->Background({0.12f, 0.12f, 0.13f, 1.0f});
    minimap->BorderWidth({1, 0, 0, 0});
    minimap->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});
    minimap->VerticalStack();
    minimap->HorizontalFixed(140);
    minimap->VerticalGrow();

    for (int i = 0; i < 80; i++) {
        auto minimapLine = std::make_unique<UIElement>();
        minimapLine->Margin(0, 0, 1, 0);

        float brightness = 0.2f + (i % 7) * 0.05f;
        minimapLine->Background({brightness, brightness, brightness + 0.05f, 1.0f});

        // Viewport indicator
        if (i >= 10 && i <= 25) {
            minimapLine->Background({0.3f, 0.4f, 0.9f, 0.3f});
        }

        minimapLine->HorizontalGrow();
        minimapLine->VerticalFixed(4);
        minimap->AddChild(std::move(minimapLine));
    }

    editorArea->AddChild(std::move(minimap));
    editorContainer->AddChild(std::move(editorArea));
    mainArea->AddChild(std::move(editorContainer));

    // ========== RIGHT PANEL (Properties/Outline) ==========
    auto rightPanel = std::make_unique<UIElement>();
    rightPanel->Background({0.12f, 0.12f, 0.13f, 1.0f});
    rightPanel->BorderWidth({1, 0, 0, 0});
    rightPanel->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});
    rightPanel->VerticalStack();
    rightPanel->HorizontalFixed(340);
    rightPanel->VerticalGrow();

    // Panel tabs
    auto panelTabs = std::make_unique<UIElement>();
    panelTabs->Padding(12);
    panelTabs->Background({0.14f, 0.14f, 0.15f, 1.0f});
    panelTabs->BorderWidth({0, 0, 1, 0});
    panelTabs->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});
    panelTabs->HorizontalStack();
    panelTabs->HorizontalGrow();
    panelTabs->VerticalFixed(56);

    for (int i = 0; i < 3; i++) {
        auto panelTab = std::make_unique<UIElement>();
        panelTab->Margin(0, 8, 0, 0);
        panelTab->Padding(10, 14, 10, 14);
        panelTab->BorderRadius({6, 6, 6, 6});

        if (i == 0) {
            panelTab->Background({0.3f, 0.4f, 0.9f, 0.15f});
            panelTab->BorderWidth({2, 2, 2, 2});
            panelTab->BorderColor({0.4f, 0.5f, 1.0f, 0.5f});
        } else {
            panelTab->Background({0.16f, 0.16f, 0.17f, 1.0f});
        }

        panelTab->HorizontalGrow();
        panelTab->VerticalFixed(32);
        panelTabs->AddChild(std::move(panelTab));
    }

    rightPanel->AddChild(std::move(panelTabs));

    // Property groups
    for (int section = 0; section < 4; section++) {
        // Section header
        auto sectionHeader = std::make_unique<UIElement>();
        sectionHeader->Padding(16, 14, 16, 14);
        sectionHeader->Background({0.14f, 0.14f, 0.15f, 1.0f});
        sectionHeader->BorderWidth({0, 0, 1, 0});
        sectionHeader->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});
        sectionHeader->HorizontalStack();
        sectionHeader->HorizontalGrow();
        sectionHeader->VerticalFixed(48);

        auto sectionTitle = std::make_unique<UIElement>();
        sectionTitle->Background({0.25f, 0.25f, 0.27f, 1.0f});
        sectionTitle->BorderRadius({4, 4, 4, 4});
        sectionTitle->HorizontalFixed(120);
        sectionTitle->VerticalFixed(22);
        sectionHeader->AddChild(std::move(sectionTitle));

        rightPanel->AddChild(std::move(sectionHeader));

        // Property items
        for (int i = 0; i < 6; i++) {
            auto propItem = std::make_unique<UIElement>();
            propItem->Padding(16, 14, 16, 14);
            propItem->HorizontalStack();
            propItem->HorizontalGrow();
            propItem->VerticalFixed(44);

            // Property label
            auto label = std::make_unique<UIElement>();
            label->Background({0.2f, 0.2f, 0.22f, 1.0f});
            label->BorderRadius({3, 3, 3, 3});
            label->HorizontalFixed(80);
            label->VerticalFixed(20);
            propItem->AddChild(std::move(label));

            auto propSpacer = std::make_unique<UIElement>();
            propSpacer->HorizontalGrow();
            propItem->AddChild(std::move(propSpacer));

            // Property value/input
            auto value = std::make_unique<UIElement>();
            value->Padding(8, 10, 8, 10);
            value->Background({0.18f, 0.18f, 0.19f, 1.0f});
            value->BorderRadius({4, 4, 4, 4});
            value->BorderWidth({1, 1, 1, 1});
            value->BorderColor({0.25f, 0.25f, 0.27f, 1.0f});
            value->HorizontalFixed(140);
            value->VerticalGrow();
            propItem->AddChild(std::move(value));

            rightPanel->AddChild(std::move(propItem));
        }
    }

    mainArea->AddChild(std::move(rightPanel));
    root->AddChild(std::move(mainArea));

    // ========== STATUS BAR ==========
    auto statusBar = std::make_unique<UIElement>();
    statusBar->Padding(12, 20, 12, 20);
    statusBar->Background({0.13f, 0.13f, 0.14f, 1.0f});
    statusBar->BorderWidth({1, 0, 0, 0});
    statusBar->BorderColor({0.2f, 0.2f, 0.22f, 1.0f});
    statusBar->BorderRadius({16, 16, 16, 16});
    statusBar->HorizontalStack();
    statusBar->HorizontalGrow();
    statusBar->VerticalFixed(52);

    // Status items
    for (int i = 0; i < 6; i++) {
        auto statusItem = std::make_unique<UIElement>();
        statusItem->Margin(0, 16, 0, 0);
        statusItem->Padding(6, 12, 6, 12);
        statusItem->BorderRadius({4, 4, 4, 4});

        if (i == 0) {
            // Warning/error indicator
            statusItem->Background({0.9f, 0.5f, 0.3f, 0.15f});
            statusItem->BorderWidth({1, 1, 1, 1});
            statusItem->BorderColor({0.9f, 0.5f, 0.3f, 0.5f});
        } else {
            statusItem->Background({0.18f, 0.18f, 0.19f, 1.0f});
        }

        statusItem->HorizontalFixed(60 + i * 15);
        statusItem->VerticalGrow();
        statusBar->AddChild(std::move(statusItem));
    }

    auto statusSpacer = std::make_unique<UIElement>();
    statusSpacer->HorizontalGrow();
    statusBar->AddChild(std::move(statusSpacer));

    // Connection indicator
    auto connectionStatus = std::make_unique<UIElement>();
    connectionStatus->Padding(8, 14, 8, 14);
    connectionStatus->Background({0.2f, 0.8f, 0.4f, 0.15f});
    connectionStatus->BorderRadius({6, 6, 6, 6});
    connectionStatus->BorderWidth({1, 1, 1, 1});
    connectionStatus->BorderColor({0.2f, 0.8f, 0.4f, 0.4f});
    connectionStatus->HorizontalStack();
    connectionStatus->HorizontalFixed(120);
    connectionStatus->VerticalGrow();

    auto statusDot = std::make_unique<UIElement>();
    statusDot->Margin(0, 8, 0, 0);
    statusDot->Background({0.2f, 0.9f, 0.4f, 1.0f});
    statusDot->BorderRadius({6, 6, 6, 6});
    statusDot->HorizontalFixed(12);
    statusDot->VerticalFixed(12);
    connectionStatus->AddChild(std::move(statusDot));

    statusBar->AddChild(std::move(connectionStatus));

    root->AddChild(std::move(statusBar));
}

    auto Window::UIStressTest() -> void {
    root = std::make_unique<UIElement>();
    root->Padding(20);
    root->Background({0.12f, 0.12f, 0.12f, 1.0f});
    root->VerticalStack();
    root->HorizontalGrow();
    root->VerticalGrow();

    // Header with multiple stat cards
    auto header = std::make_unique<UIElement>();
    header->Padding(0, 0, 20, 0);
    header->HorizontalStack();
    header->HorizontalGrow();
    header->VerticalFit();

    // Create 8 stat cards
    for (int i = 0; i < 8; i++) {
        auto card = std::make_unique<UIElement>();
        card->Margin(0, 10, 0, 0);
        card->Padding(16);
        card->Background({0.18f, 0.18f, 0.18f, 1.0f});
        card->BorderRadius({8, 8, 8, 8});
        card->BoxShadow({0, 4}, 12, 0, {0.0f, 0.0f, 0.0f, 0.3f});
        card->VerticalStack();
        card->HorizontalGrow();
        card->VerticalFit();

        // Card header
        auto cardHeader = std::make_unique<UIElement>();
        cardHeader->Padding(0, 0, 8, 0);
        cardHeader->Background({0.2f, 0.2f, 0.2f, 1.0f});
        cardHeader->BorderRadius({4, 4, 4, 4});
        cardHeader->HorizontalGrow();
        cardHeader->VerticalFixed(24);
        card->AddChild(std::move(cardHeader));

        // Card value (large number)
        auto cardValue = std::make_unique<UIElement>();
        cardValue->Padding(12, 0, 8, 0);
        cardValue->Background({0.22f, 0.22f, 0.22f, 1.0f});
        cardValue->BorderRadius({4, 4, 4, 4});
        cardValue->HorizontalGrow();
        cardValue->VerticalFixed(40);
        card->AddChild(std::move(cardValue));

        // Mini chart bars
        auto miniChart = std::make_unique<UIElement>();
        miniChart->Padding(8, 0, 0, 0);
        miniChart->HorizontalStack();
        miniChart->HorizontalGrow();
        miniChart->VerticalFixed(32);

        for (int j = 0; j < 12; j++) {
            auto bar = std::make_unique<UIElement>();
            bar->Margin(0, 2, 0, 0);
            bar->Background({0.0f + j * 0.05f, 0.5f, 1.0f, 0.8f});
            bar->BorderRadius({2, 2, 2, 2});
            bar->HorizontalGrow();
            bar->VerticalFixed(8 + (j % 4) * 6);
            miniChart->AddChild(std::move(bar));
        }
        card->AddChild(std::move(miniChart));

        header->AddChild(std::move(card));
    }
    root->AddChild(std::move(header));

    // Main content area with grid
    auto mainContent = std::make_unique<UIElement>();
    mainContent->HorizontalStack();
    mainContent->HorizontalGrow();
    mainContent->VerticalGrow();

    // Left panel - nested list with 100 items
    auto leftPanel = std::make_unique<UIElement>();
    leftPanel->Padding(16);
    leftPanel->Background({0.15f, 0.15f, 0.15f, 1.0f});
    leftPanel->BorderRadius({8, 8, 8, 8});
    leftPanel->VerticalStack();
    leftPanel->HorizontalFixed(280);
    leftPanel->VerticalGrow();

    for (int i = 0; i < 100; i++) {
        auto listItem = std::make_unique<UIElement>();
        listItem->Padding(8);
        listItem->Margin(0, 0, 2, 0);

        if (i % 7 == 0) {
            listItem->Background({0.0f, 0.48f, 1.0f, 0.2f});
        } else {
            listItem->Background({0.18f, 0.18f, 0.18f, 1.0f});
        }

        listItem->BorderRadius({4, 4, 4, 4});
        listItem->HorizontalStack();
        listItem->HorizontalGrow();
        listItem->VerticalFixed(32);

        // Icon placeholder
        auto icon = std::make_unique<UIElement>();
        icon->Background({0.3f, 0.6f, 1.0f, 1.0f});
        icon->BorderRadius({4, 4, 4, 4});
        icon->HorizontalFixed(20);
        icon->VerticalFixed(20);
        listItem->AddChild(std::move(icon));

        // Badge
        auto badge = std::make_unique<UIElement>();
        badge->Margin(0, 0, 0, 8);
        badge->Background({1.0f, 0.3f, 0.3f, 1.0f});
        badge->BorderRadius({8, 8, 8, 8});
        badge->HorizontalFixed(20);
        badge->VerticalFixed(16);
        listItem->AddChild(std::move(badge));

        leftPanel->AddChild(std::move(listItem));
    }
    mainContent->AddChild(std::move(leftPanel));

    // Center - grid of cards (10x10 = 100 cards)
    auto centerPanel = std::make_unique<UIElement>();
    centerPanel->Padding(0, 16, 0, 16);
    centerPanel->VerticalStack();
    centerPanel->HorizontalGrow();
    centerPanel->VerticalGrow();

    for (int row = 0; row < 10; row++) {
        auto rowContainer = std::make_unique<UIElement>();
        rowContainer->Padding(0, 0, 8, 0);
        rowContainer->HorizontalStack();
        rowContainer->HorizontalGrow();
        rowContainer->VerticalFit();

        for (int col = 0; col < 10; col++) {
            auto gridCard = std::make_unique<UIElement>();
            gridCard->Margin(0, 8, 0, 0);
            gridCard->Padding(12);
            gridCard->Background({0.16f, 0.16f, 0.16f, 1.0f});
            gridCard->BorderRadius({6, 6, 6, 6});
            gridCard->BorderWidth({1, 1, 1, 1});
            gridCard->BorderColor({0.25f, 0.25f, 0.25f, 1.0f});
            gridCard->VerticalStack();
            gridCard->HorizontalGrow();
            gridCard->VerticalFit();

            // Card image placeholder
            auto cardImage = std::make_unique<UIElement>();
            cardImage->Background({0.2f + col * 0.05f, 0.3f + row * 0.05f, 0.8f, 1.0f});
            cardImage->BorderRadius({4, 4, 4, 4});
            cardImage->HorizontalGrow();
            cardImage->VerticalFixed(60);
            gridCard->AddChild(std::move(cardImage));

            // Card title
            auto cardTitle = std::make_unique<UIElement>();
            cardTitle->Padding(8, 0, 4, 0);
            cardTitle->Background({0.22f, 0.22f, 0.22f, 1.0f});
            cardTitle->BorderRadius({3, 3, 3, 3});
            cardTitle->HorizontalGrow();
            cardTitle->VerticalFixed(20);
            gridCard->AddChild(std::move(cardTitle));

            // Card actions (3 buttons)
            auto cardActions = std::make_unique<UIElement>();
            cardActions->Padding(4, 0, 0, 0);
            cardActions->HorizontalStack();
            cardActions->HorizontalGrow();
            cardActions->VerticalFit();

            for (int btn = 0; btn < 3; btn++) {
                auto button = std::make_unique<UIElement>();
                button->Margin(0, 4, 0, 0);
                button->Background({0.25f, 0.25f, 0.25f, 1.0f});
                button->BorderRadius({4, 4, 4, 4});
                button->HorizontalGrow();
                button->VerticalFixed(24);
                cardActions->AddChild(std::move(button));
            }
            gridCard->AddChild(std::move(cardActions));

            rowContainer->AddChild(std::move(gridCard));
        }
        centerPanel->AddChild(std::move(rowContainer));
    }
    mainContent->AddChild(std::move(centerPanel));

    // Right panel - activity feed with 80 items
    auto rightPanel = std::make_unique<UIElement>();
    rightPanel->Padding(16);
    rightPanel->Background({0.15f, 0.15f, 0.15f, 1.0f});
    rightPanel->BorderRadius({8, 8, 8, 8});
    rightPanel->VerticalStack();
    rightPanel->HorizontalFixed(300);
    rightPanel->VerticalGrow();

    for (int i = 0; i < 80; i++) {
        auto activity = std::make_unique<UIElement>();
        activity->Padding(12);
        activity->Margin(0, 0, 4, 0);
        activity->Background({0.18f, 0.18f, 0.18f, 1.0f});
        activity->BorderRadius({6, 6, 6, 6});
        activity->BorderWidth({0, 0, 0, 3});

        float hue = (i % 5) * 0.2f;
        activity->BorderColor({hue, 0.5f, 1.0f, 1.0f});

        activity->VerticalStack();
        activity->HorizontalGrow();
        activity->VerticalFit();

        // Activity header
        auto actHeader = std::make_unique<UIElement>();
        actHeader->Padding(0, 0, 6, 0);
        actHeader->HorizontalStack();
        actHeader->HorizontalGrow();
        actHeader->VerticalFit();

        auto actAvatar = std::make_unique<UIElement>();
        actAvatar->Background({0.4f, 0.6f, 0.9f, 1.0f});
        actAvatar->BorderRadius({12, 12, 12, 12});
        actAvatar->HorizontalFixed(24);
        actAvatar->VerticalFixed(24);
        actHeader->AddChild(std::move(actAvatar));

        auto actTime = std::make_unique<UIElement>();
        actTime->Margin(0, 0, 0, 8);
        actTime->Background({0.2f, 0.2f, 0.2f, 1.0f});
        actTime->BorderRadius({3, 3, 3, 3});
        actTime->HorizontalFixed(60);
        actTime->VerticalFixed(18);
        actHeader->AddChild(std::move(actTime));

        activity->AddChild(std::move(actHeader));

        // Activity content
        auto actContent = std::make_unique<UIElement>();
        actContent->Padding(6, 0, 0, 0);
        actContent->Background({0.2f, 0.2f, 0.2f, 1.0f});
        actContent->BorderRadius({4, 4, 4, 4});
        actContent->HorizontalGrow();
        actContent->VerticalFixed(40);
        activity->AddChild(std::move(actContent));

        rightPanel->AddChild(std::move(activity));
    }
    mainContent->AddChild(std::move(rightPanel));

    root->AddChild(std::move(mainContent));
}

}
