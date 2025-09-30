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
        UIStressTest();
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
titleBar->BorderRadius({12, 12, 0, 0});
titleBar->HorizontalStack();
titleBar->HorizontalGrow();
titleBar->VerticalFixed(52);

// Traffic lights container
auto trafficLights = std::make_unique<UIElement>();
trafficLights->HorizontalStack();
trafficLights->HorizontalFixed(68);
trafficLights->VerticalGrow();

// Red button
auto redButton = std::make_unique<UIElement>();
redButton->Margin(0, 8, 0, 0);
redButton->Background({1.0f, 0.38f, 0.38f, 1.0f});
redButton->BorderRadius({6, 6, 6, 6});
redButton->HorizontalFixed(12);
redButton->VerticalFixed(12);
trafficLights->AddChild(std::move(redButton));

// Yellow button
auto yellowButton = std::make_unique<UIElement>();
yellowButton->Margin(0, 8, 0, 0);
yellowButton->Background({1.0f, 0.76f, 0.28f, 1.0f});
yellowButton->BorderRadius({6, 6, 6, 6});
yellowButton->HorizontalFixed(12);
yellowButton->VerticalFixed(12);
trafficLights->AddChild(std::move(yellowButton));

// Green button
auto greenButton = std::make_unique<UIElement>();
greenButton->Background({0.27f, 0.84f, 0.35f, 1.0f});
greenButton->BorderRadius({6, 6, 6, 6});
greenButton->HorizontalFixed(12);
greenButton->VerticalFixed(12);
trafficLights->AddChild(std::move(greenButton));

titleBar->AddChild(std::move(trafficLights));

// Window title (center)
auto titleSpace = std::make_unique<UIElement>();
titleSpace->HorizontalGrow();
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
bottomPanel->BorderRadius({0, 0, 12, 12});
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
consoleContent->VerticalStack();
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

    auto Window::Render(Graphic::GL::UIRenderer& renderer, Graphic::GL::TextRenderer& text_render, float target_x, float target_y, const glm::vec2& screenSize) -> void {
        root->CalculateFitSizeOnAxis(Axis::Horizontal);
        root->CalculateGrowSizeOnAxis(Axis::Horizontal);

        root->CalculateFitSizeOnAxis(Axis::Vertical);
        root->CalculateGrowSizeOnAxis(Axis::Vertical);

        root->PositionOnAxis(Axis::Horizontal, 0);
        root->PositionOnAxis(Axis::Vertical, 0);



        root->GlobalPositionPass(target_x, target_y);
        root->CullingPass(vec4(0 , 0, screenSize.x, screenSize.y));
        root->RenderQueue(renderer,text_render, target_x, target_y);

        root->MarginLeft((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
        root->MarginRight((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);

    }

    auto Window::RenderProfile(Graphic::GL::UIRenderer& renderer, Graphic::GL::TextRenderer& text_render,
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
    root->RenderQueue(renderer, text_render, target_x, target_y);
    end = std::chrono::high_resolution_clock::now();
    double renderQueue = std::chrono::duration<double, std::milli>(end - start).count();

    printf("FitX: %.3f | GrowX: %.3f | FitY: %.3f | GrowY: %.3f | PosX: %.3f | PosY: %.3f | Global: %.3f | Cull: %.3f | Queue: %.3f\n",
           fitX, growX, fitY, growY, posX, posY, globalPos, culling, renderQueue);

    root->MarginLeft((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
    root->MarginRight((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
}

    auto Window::InitLayout(float target_x, float target_y, const glm::vec2& screenSize) -> void {
        root->GlobalPositionPass(target_x, target_y);
        root->CullingPass(vec4(0 , 0, screenSize.x, screenSize.y));
        root->MarginLeft((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
        root->MarginRight((screenSize.x - root->borderBoxOnAxis(Axis::Horizontal))/2);
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
}
