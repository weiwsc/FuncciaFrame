//
// Created by Wangsicong Wei on 2025-09-26.
//

#include "Window.h"
#include "UIElement.h"
#include <chrono>

#include "../graphic/gl/TextRenderer.h"

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

        TestUI();
        //MockUI();
    }





    auto Window::TextUI() -> void {
            root = std::make_unique<UIElement>();
    root->Padding(24);
    root->Background({0.11f, 0.11f, 0.12f, 1.0f});
    root->VerticalStack();
    root->HorizontalFixed(2000);
    root->VerticalFixed(1000);


    // ================= TITLE BAR =================
    auto titleBar = std::make_unique<UIElement>();
    titleBar->Padding(16, 24, 16, 24);
    titleBar->Background({0.14f, 0.14f, 0.15f, 1.0f});
    titleBar->HorizontalStack();
    titleBar->HorizontalGrow();
    titleBar->VerticalFixed(96);

    auto titleText = std::make_unique<UIElement>();
    titleText->Text("Funccia Editor");
    titleText->FontSize(44);
    titleText->TextColor({0.9f, 0.9f, 0.92f, 1.0f});
    titleText->HorizontalGrow();
    titleText->VerticalGrow();
    titleBar->AddChild(std::move(titleText));

    root->AddChild(std::move(titleBar));

    // ================= MAIN AREA =================
    auto mainArea = std::make_unique<UIElement>();
    mainArea->HorizontalStack();
    mainArea->HorizontalGrow();
    mainArea->VerticalGrow();

    // -------- Sidebar --------
    auto sidebar = std::make_unique<UIElement>();
    sidebar->Padding(12);
    sidebar->Background({0.12f, 0.12f, 0.13f, 1.0f});
    sidebar->VerticalStack();
    sidebar->HorizontalFixed(360);
    sidebar->VerticalGrow();

    auto sidebarHeader = std::make_unique<UIElement>();
    sidebarHeader->Text("Project");
    sidebarHeader->FontSize(42);
    sidebarHeader->TextColor({0.85f, 0.85f, 0.88f, 1.0f});
    sidebarHeader->MarginBottom(12);
    sidebar->AddChild(std::move(sidebarHeader));

    for (int i = 0; i < 6; ++i) {
        auto item = std::make_unique<UIElement>();
        item->Padding(12, 16, 12, 16);
        item->Background(i == 2
            ? vec4{0.3f, 0.4f, 0.9f, 0.2f}
            : vec4{0.18f, 0.18f, 0.19f, 1.0f});
        item->BorderRadius({6, 6, 6, 6});
        item->VerticalFixed(64);

        auto label = std::make_unique<UIElement>();
        label->Text(i == 2 ? "main.cpp" : "file.cpp");
        label->FontSize(42);
        label->TextColor({0.9f, 0.9f, 0.92f, 1.0f});
        item->AddChild(std::move(label));

        sidebar->AddChild(std::move(item));
    }

    mainArea->AddChild(std::move(sidebar));

    // -------- Editor --------
    auto editor = std::make_unique<UIElement>();
    editor->Padding(24);
    editor->Background({0.15f, 0.15f, 0.16f, 1.0f});
    editor->VerticalStack();
    editor->HorizontalGrow();
    editor->VerticalGrow();

    auto editorTitle = std::make_unique<UIElement>();
    editorTitle->Text("main.cpp");
    editorTitle->FontSize(44);
    editorTitle->TextColor({0.9f, 0.9f, 0.92f, 1.0f});
    editorTitle->MarginBottom(16);
    editor->AddChild(std::move(editorTitle));

    for (int i = 0; i < 8; ++i) {
        auto line = std::make_unique<UIElement>();
        line->Padding(8, 12, 8, 12);
        line->Background(i == 3
            ? vec4{0.3f, 0.4f, 0.9f, 0.12f}
            : vec4{0, 0, 0, 0});
        line->HorizontalGrow();
        line->VerticalFixed(64);

        auto code = std::make_unique<UIElement>();
        code->Text("auto value = ComputeSomething();");
        code->FontSize(40); // slightly smaller for code
        code->TextColor({0.85f, 0.85f, 0.88f, 1.0f});
        line->AddChild(std::move(code));

        editor->AddChild(std::move(line));
    }

    mainArea->AddChild(std::move(editor));
    root->AddChild(std::move(mainArea));

    // ================= STATUS BAR =================
    auto statusBar = std::make_unique<UIElement>();
    statusBar->Padding(12, 20, 12, 20);
    statusBar->Background({0.13f, 0.13f, 0.14f, 1.0f});
    statusBar->HorizontalStack();
    statusBar->HorizontalGrow();
    statusBar->VerticalFixed(72);

    auto statusText = std::make_unique<UIElement>();
    statusText->Text("Build succeeded");
    statusText->FontSize(42);
    statusText->TextColor({0.6f, 0.9f, 0.6f, 1.0f});
    statusText->HorizontalGrow();
    statusBar->AddChild(std::move(statusText));

    root->AddChild(std::move(statusBar));
    }

    auto Window::MockUI() -> void {
    root = std::make_unique<UIElement>();
    root->Padding(0);
    root->Background({0.09f, 0.09f, 0.10f, 1.0f});
    root->VerticalStack();
    root->HorizontalFixed(2400);
    root->VerticalFixed(1800);

    // ========== TOP NAVIGATION BAR ==========
    auto navBar = std::make_unique<UIElement>();
    navBar->Padding(24, 40, 24, 40);
    navBar->Background({0.12f, 0.12f, 0.13f, 1.0f});
    navBar->BorderWidth({0, 0, 2, 0});
    navBar->BorderColor({0.18f, 0.18f, 0.20f, 1.0f});
    navBar->HorizontalStack();
    navBar->HorizontalGrow();
    navBar->VerticalFixed(100);

    // Logo/Brand
    auto logo = std::make_unique<UIElement>();
    logo->Background({0.4f, 0.6f, 1.0f, 1.0f});
    logo->BorderRadius({12, 12, 12, 12});
    logo->HorizontalFixed(52);
    logo->VerticalFixed(52);
    navBar->AddChild(std::move(logo));

    auto brandName = std::make_unique<UIElement>();
    brandName->MarginLeft(20);
    brandName->Padding(10, 0, 10, 0);
    brandName->Text("Dashboard");
    brandName->FontSize(44);
    brandName->TextColor({0.95f, 0.95f, 0.97f, 1.0f});
    brandName->HorizontalFixed(220);
    brandName->VerticalGrow();
    navBar->AddChild(std::move(brandName));

    auto navSpacer = std::make_unique<UIElement>();
    navSpacer->HorizontalGrow();
    navBar->AddChild(std::move(navSpacer));

    // Nav Links
    const char* navItems[] = {"Overview", "Analytics", "Projects", "Settings"};
    for (int i = 0; i < 4; i++) {
        auto navItem = std::make_unique<UIElement>();
        navItem->Margin(0, 16, 0, 16);
        navItem->Padding(16, 28, 16, 28);
        navItem->BorderRadius({10, 10, 10, 10});

        if (i == 0) {
            navItem->Background({0.4f, 0.6f, 1.0f, 0.15f});
            navItem->TextColor({0.5f, 0.7f, 1.0f, 1.0f});
        } else {
            navItem->TextColor({0.6f, 0.6f, 0.65f, 1.0f});
        }

        navItem->Text(navItems[i]);
        navItem->FontSize(38);
        navItem->HorizontalFit();
        navItem->VerticalGrow();
        navBar->AddChild(std::move(navItem));
    }

    auto navSpacer2 = std::make_unique<UIElement>();
    navSpacer2->HorizontalGrow();
    navBar->AddChild(std::move(navSpacer2));

    // Search Bar
    auto searchContainer = std::make_unique<UIElement>();
    searchContainer->Padding(16, 24, 16, 24);
    searchContainer->Background({0.16f, 0.16f, 0.18f, 1.0f});
    searchContainer->BorderRadius({12, 12, 12, 12});
    searchContainer->BorderWidth({2, 2, 2, 2});
    searchContainer->BorderColor({0.22f, 0.22f, 0.25f, 1.0f});
    searchContainer->HorizontalStack();
    searchContainer->HorizontalFixed(380);
    searchContainer->VerticalGrow();

    auto searchIcon = std::make_unique<UIElement>();
    searchIcon->Background({0.45f, 0.45f, 0.50f, 1.0f});
    searchIcon->BorderRadius({6, 6, 6, 6});
    searchIcon->HorizontalFixed(28);
    searchIcon->VerticalFixed(28);
    searchContainer->AddChild(std::move(searchIcon));

    auto searchText = std::make_unique<UIElement>();
    searchText->MarginLeft(16);
    searchText->Padding(4, 0, 4, 0);
    searchText->Text("Search...");
    searchText->FontSize(36);
    searchText->TextColor({0.45f, 0.45f, 0.50f, 1.0f});
    searchText->HorizontalGrow();
    searchText->VerticalGrow();
    searchContainer->AddChild(std::move(searchText));

    navBar->AddChild(std::move(searchContainer));

    // User Avatar
    auto userAvatar = std::make_unique<UIElement>();
    userAvatar->MarginLeft(28);
    userAvatar->Background({0.9f, 0.5f, 0.6f, 1.0f});
    userAvatar->BorderRadius({26, 26, 26, 26});
    userAvatar->BorderWidth({3, 3, 3, 3});
    userAvatar->BorderColor({0.95f, 0.6f, 0.7f, 0.4f});
    userAvatar->HorizontalFixed(52);
    userAvatar->VerticalFixed(52);
    navBar->AddChild(std::move(userAvatar));

    root->AddChild(std::move(navBar));

    // ========== MAIN CONTENT AREA ==========
    auto mainContent = std::make_unique<UIElement>();
    mainContent->Padding(40);
    mainContent->HorizontalStack();
    mainContent->HorizontalGrow();
    mainContent->VerticalGrow();

    // ========== LEFT SIDEBAR ==========
    auto sidebar = std::make_unique<UIElement>();
    sidebar->Padding(28);
    sidebar->Background({0.11f, 0.11f, 0.12f, 1.0f});
    sidebar->BorderRadius({20, 20, 20, 20});
    sidebar->VerticalStack();
    sidebar->HorizontalFixed(340);
    sidebar->VerticalGrow();

    // Sidebar Title
    auto sidebarTitle = std::make_unique<UIElement>();
    sidebarTitle->Padding(12, 8, 20, 8);
    sidebarTitle->Text("Quick Actions");
    sidebarTitle->FontSize(34);
    sidebarTitle->TextColor({0.5f, 0.5f, 0.55f, 1.0f});
    sidebarTitle->HorizontalGrow();
    sidebarTitle->VerticalFixed(70);
    sidebar->AddChild(std::move(sidebarTitle));

    // Sidebar Menu Items
    const char* menuItems[] = {"New Project", "Import Data", "Export Report", "Team Chat", "Calendar", "Documents"};
    vec4 menuColors[] = {
        {0.4f, 0.8f, 0.6f, 1.0f},
        {0.5f, 0.6f, 1.0f, 1.0f},
        {1.0f, 0.6f, 0.4f, 1.0f},
        {0.9f, 0.5f, 0.8f, 1.0f},
        {0.4f, 0.8f, 0.9f, 1.0f},
        {0.9f, 0.8f, 0.4f, 1.0f}
    };

    for (int i = 0; i < 6; i++) {
        auto menuItem = std::make_unique<UIElement>();
        menuItem->Margin(6, 0, 6, 0);
        menuItem->Padding(20, 24, 20, 24);
        menuItem->BorderRadius({14, 14, 14, 14});
        menuItem->HorizontalStack();

        if (i == 0) {
            menuItem->Background({0.4f, 0.8f, 0.6f, 0.12f});
            menuItem->BorderWidth({2, 2, 2, 2});
            menuItem->BorderColor({0.4f, 0.8f, 0.6f, 0.3f});
        }

        menuItem->HorizontalGrow();
        menuItem->VerticalFixed(80);

        auto menuIcon = std::make_unique<UIElement>();
        menuIcon->Background(menuColors[i]);
        menuIcon->BorderRadius({10, 10, 10, 10});
        menuIcon->HorizontalFixed(36);
        menuIcon->VerticalFixed(36);
        menuItem->AddChild(std::move(menuIcon));

        auto menuText = std::make_unique<UIElement>();
        menuText->MarginLeft(20);
        menuText->Padding(8, 0, 8, 0);
        menuText->Text(menuItems[i]);
        menuText->FontSize(38);
        menuText->TextColor({0.85f, 0.85f, 0.88f, 1.0f});
        menuText->HorizontalGrow();
        menuText->VerticalGrow();
        menuItem->AddChild(std::move(menuText));

        sidebar->AddChild(std::move(menuItem));
    }

    // Sidebar spacer
    auto sidebarSpacer = std::make_unique<UIElement>();
    sidebarSpacer->VerticalGrow();
    sidebar->AddChild(std::move(sidebarSpacer));

    // Storage indicator
    auto storageCard = std::make_unique<UIElement>();
    storageCard->Padding(24);
    storageCard->Background({0.14f, 0.14f, 0.16f, 1.0f});
    storageCard->BorderRadius({16, 16, 16, 16});
    storageCard->VerticalStack();
    storageCard->HorizontalGrow();
    storageCard->VerticalFixed(180);

    auto storageTitle = std::make_unique<UIElement>();
    storageTitle->Padding(4, 0, 12, 0);
    storageTitle->Text("Storage Used");
    storageTitle->FontSize(34);
    storageTitle->TextColor({0.6f, 0.6f, 0.65f, 1.0f});
    storageTitle->HorizontalGrow();
    storageTitle->VerticalFixed(56);
    storageCard->AddChild(std::move(storageTitle));

    auto storageBarBg = std::make_unique<UIElement>();
    storageBarBg->Background({0.2f, 0.2f, 0.22f, 1.0f});
    storageBarBg->BorderRadius({10, 10, 10, 10});
    storageBarBg->HorizontalGrow();
    storageBarBg->VerticalFixed(20);
    storageCard->AddChild(std::move(storageBarBg));

    auto storageFill = std::make_unique<UIElement>();
    storageFill->MarginTop(12);
    storageFill->Background({0.4f, 0.6f, 1.0f, 1.0f});
    storageFill->BorderRadius({10, 10, 10, 10});
    storageFill->HorizontalFixed(180);
    storageFill->VerticalFixed(20);
    storageCard->AddChild(std::move(storageFill));

    auto storageText = std::make_unique<UIElement>();
    storageText->MarginTop(16);
    storageText->Padding(4, 0, 4, 0);
    storageText->Text("6.4 GB of 10 GB");
    storageText->FontSize(32);
    storageText->TextColor({0.5f, 0.5f, 0.55f, 1.0f});
    storageText->HorizontalGrow();
    storageText->VerticalGrow();
    storageCard->AddChild(std::move(storageText));

    sidebar->AddChild(std::move(storageCard));

    mainContent->AddChild(std::move(sidebar));

    // ========== CENTER CONTENT ==========
    auto centerContent = std::make_unique<UIElement>();
    centerContent->MarginLeft(40);
    centerContent->VerticalStack();
    centerContent->HorizontalGrow();
    centerContent->VerticalGrow();

    // Welcome Header
    auto welcomeSection = std::make_unique<UIElement>();
    welcomeSection->Padding(36);
    welcomeSection->Background({0.11f, 0.11f, 0.12f, 1.0f});
    welcomeSection->BorderRadius({20, 20, 20, 20});
    welcomeSection->HorizontalStack();
    welcomeSection->HorizontalGrow();
    welcomeSection->VerticalFixed(200);

    auto welcomeText = std::make_unique<UIElement>();
    welcomeText->VerticalStack();
    welcomeText->HorizontalGrow();
    welcomeText->VerticalGrow();

    auto welcomeTitle = std::make_unique<UIElement>();
    welcomeTitle->Padding(8, 0, 12, 0);
    welcomeTitle->Text("Welcome back, Alex!");
    welcomeTitle->FontSize(48);
    welcomeTitle->TextColor({0.95f, 0.95f, 0.97f, 1.0f});
    welcomeTitle->HorizontalGrow();
    welcomeTitle->VerticalFixed(80);
    welcomeText->AddChild(std::move(welcomeTitle));

    auto welcomeSubtitle = std::make_unique<UIElement>();
    welcomeSubtitle->Padding(8, 0, 8, 0);
    welcomeSubtitle->Text("You have 4 tasks pending and 2 meetings today.");
    welcomeSubtitle->FontSize(36);
    welcomeSubtitle->TextColor({0.55f, 0.55f, 0.60f, 1.0f});
    welcomeSubtitle->HorizontalGrow();
    welcomeSubtitle->VerticalGrow();
    welcomeText->AddChild(std::move(welcomeSubtitle));

    welcomeSection->AddChild(std::move(welcomeText));

    // Action Button
    auto actionBtn = std::make_unique<UIElement>();
    actionBtn->Padding(24, 40, 24, 40);
    actionBtn->Background({0.4f, 0.6f, 1.0f, 1.0f});
    actionBtn->BorderRadius({14, 14, 14, 14});
    actionBtn->BoxShadow({0, 8}, 24, 0, {0.4f, 0.6f, 1.0f, 0.3f});
    actionBtn->Text("View Tasks");
    actionBtn->FontSize(38);
    actionBtn->TextColor({1.0f, 1.0f, 1.0f, 1.0f});
    actionBtn->HorizontalFixed(220);
    actionBtn->VerticalFixed(80);
    welcomeSection->AddChild(std::move(actionBtn));

    centerContent->AddChild(std::move(welcomeSection));

    // ========== STATS CARDS ROW ==========
    auto statsRow = std::make_unique<UIElement>();
    statsRow->MarginTop(32);
    statsRow->HorizontalStack();
    statsRow->HorizontalGrow();
    statsRow->VerticalFixed(220);

    struct StatCard {
        const char* title;
        const char* value;
        const char* change;
        vec4 accentColor;
        bool positive;
    };

    StatCard stats[] = {
        {"Total Projects", "24", "+3 this week", {0.4f, 0.8f, 0.6f, 1.0f}, true},
        {"Active Tasks", "156", "+12 pending", {0.5f, 0.6f, 1.0f, 1.0f}, true},
        {"Team Members", "18", "+2 joined", {0.9f, 0.5f, 0.8f, 1.0f}, true},
        {"Hours Logged", "847", "-5% vs last", {1.0f, 0.6f, 0.4f, 1.0f}, false}
    };

    for (int i = 0; i < 4; i++) {
        auto statCard = std::make_unique<UIElement>();
        if (i > 0) statCard->MarginLeft(24);
        statCard->Padding(28);
        statCard->Background({0.11f, 0.11f, 0.12f, 1.0f});
        statCard->BorderRadius({20, 20, 20, 20});
        statCard->BorderWidth({0, 0, 0, 4});
        statCard->BorderColor(stats[i].accentColor);
        statCard->VerticalStack();
        statCard->HorizontalGrow();
        statCard->VerticalGrow();

        auto statTitle = std::make_unique<UIElement>();
        statTitle->Padding(4, 0, 8, 0);
        statTitle->Text(stats[i].title);
        statTitle->FontSize(32);
        statTitle->TextColor({0.55f, 0.55f, 0.60f, 1.0f});
        statTitle->HorizontalGrow();
        statTitle->VerticalFixed(52);
        statCard->AddChild(std::move(statTitle));

        auto statValue = std::make_unique<UIElement>();
        statValue->Padding(4, 0, 4, 0);
        statValue->Text(stats[i].value);
        statValue->FontSize(56);
        statValue->TextColor({0.95f, 0.95f, 0.97f, 1.0f});
        statValue->HorizontalGrow();
        statValue->VerticalFixed(80);
        statCard->AddChild(std::move(statValue));

        auto statChange = std::make_unique<UIElement>();
        statChange->Padding(4, 0, 4, 0);
        statChange->Text(stats[i].change);
        statChange->FontSize(30);
        statChange->TextColor(stats[i].positive ? vec4{0.4f, 0.8f, 0.6f, 1.0f} : vec4{1.0f, 0.5f, 0.4f, 1.0f});
        statChange->HorizontalGrow();
        statChange->VerticalGrow();
        statCard->AddChild(std::move(statChange));

        statsRow->AddChild(std::move(statCard));
    }

    centerContent->AddChild(std::move(statsRow));

    // ========== RECENT ACTIVITY SECTION ==========
    auto activitySection = std::make_unique<UIElement>();
    activitySection->MarginTop(32);
    activitySection->Padding(32);
    activitySection->Background({0.11f, 0.11f, 0.12f, 1.0f});
    activitySection->BorderRadius({20, 20, 20, 20});
    activitySection->VerticalStack();
    activitySection->HorizontalGrow();
    activitySection->VerticalGrow();

    // Section Header
    auto activityHeader = std::make_unique<UIElement>();
    activityHeader->HorizontalStack();
    activityHeader->HorizontalGrow();
    activityHeader->VerticalFixed(80);

    auto activityTitle = std::make_unique<UIElement>();
    activityTitle->Padding(8, 0, 8, 0);
    activityTitle->Text("Recent Activity");
    activityTitle->FontSize(44);
    activityTitle->TextColor({0.95f, 0.95f, 0.97f, 1.0f});
    activityTitle->HorizontalGrow();
    activityTitle->VerticalGrow();
    activityHeader->AddChild(std::move(activityTitle));

    auto viewAllBtn = std::make_unique<UIElement>();
    viewAllBtn->Padding(16, 28, 16, 28);
    viewAllBtn->Background({0.16f, 0.16f, 0.18f, 1.0f});
    viewAllBtn->BorderRadius({12, 12, 12, 12});
    viewAllBtn->Text("View All");
    viewAllBtn->FontSize(34);
    viewAllBtn->TextColor({0.5f, 0.7f, 1.0f, 1.0f});
    viewAllBtn->HorizontalFixed(180);
    viewAllBtn->VerticalFixed(64);
    activityHeader->AddChild(std::move(viewAllBtn));

    activitySection->AddChild(std::move(activityHeader));

    // Activity Items
    struct ActivityItem {
        const char* user;
        const char* action;
        const char* time;
        vec4 avatarColor;
    };

    ActivityItem activities[] = {
        {"Sarah Chen", "completed 'API Integration'", "2 min ago", {0.9f, 0.5f, 0.6f, 1.0f}},
        {"Mike Johnson", "commented on 'Dashboard'", "15 min ago", {0.4f, 0.7f, 0.9f, 1.0f}},
        {"Emily Davis", "created project 'Mobile App'", "1 hour ago", {0.6f, 0.9f, 0.5f, 1.0f}},
        {"Alex Wong", "uploaded 3 files to 'Assets'", "2 hours ago", {0.9f, 0.7f, 0.4f, 1.0f}}
    };

    for (int i = 0; i < 4; i++) {
        auto activityItem = std::make_unique<UIElement>();
        activityItem->MarginTop(16);
        activityItem->Padding(24, 28, 24, 28);
        activityItem->Background({0.13f, 0.13f, 0.14f, 1.0f});
        activityItem->BorderRadius({14, 14, 14, 14});
        activityItem->HorizontalStack();
        activityItem->HorizontalGrow();
        activityItem->VerticalFixed(100);

        // User Avatar
        auto avatar = std::make_unique<UIElement>();
        avatar->Background(activities[i].avatarColor);
        avatar->BorderRadius({24, 24, 24, 24});
        avatar->HorizontalFixed(52);
        avatar->VerticalFixed(52);
        activityItem->AddChild(std::move(avatar));

        // Activity Details
        auto details = std::make_unique<UIElement>();
        details->MarginLeft(24);
        details->VerticalStack();
        details->HorizontalGrow();
        details->VerticalGrow();

        auto userName = std::make_unique<UIElement>();
        userName->Padding(4, 0, 4, 0);
        userName->Text(activities[i].user);
        userName->FontSize(38);
        userName->TextColor({0.9f, 0.9f, 0.92f, 1.0f});
        userName->HorizontalGrow();
        userName->VerticalFixed(52);
        details->AddChild(std::move(userName));

        auto actionText = std::make_unique<UIElement>();
        actionText->Padding(4, 0, 4, 0);
        actionText->Text(activities[i].action);
        actionText->FontSize(32);
        actionText->TextColor({0.55f, 0.55f, 0.60f, 1.0f});
        actionText->HorizontalGrow();
        actionText->VerticalGrow();
        details->AddChild(std::move(actionText));

        activityItem->AddChild(std::move(details));

        // Timestamp
        auto timestamp = std::make_unique<UIElement>();
        timestamp->Padding(8, 16, 8, 16);
        timestamp->Text(activities[i].time);
        timestamp->FontSize(30);
        timestamp->TextColor({0.45f, 0.45f, 0.50f, 1.0f});
        timestamp->HorizontalFixed(180);
        timestamp->VerticalFixed(52);
        activityItem->AddChild(std::move(timestamp));

        activitySection->AddChild(std::move(activityItem));
    }

    centerContent->AddChild(std::move(activitySection));
    mainContent->AddChild(std::move(centerContent));

    // ========== RIGHT PANEL ==========
    auto rightPanel = std::make_unique<UIElement>();
    rightPanel->MarginLeft(40);
    rightPanel->Padding(32);
    rightPanel->Background({0.11f, 0.11f, 0.12f, 1.0f});
    rightPanel->BorderRadius({20, 20, 20, 20});
    rightPanel->VerticalStack();
    rightPanel->HorizontalFixed(420);
    rightPanel->VerticalGrow();

    // Calendar Widget Title
    auto calendarTitle = std::make_unique<UIElement>();
    calendarTitle->Padding(8, 0, 12, 0);
    calendarTitle->Text("Today's Schedule");
    calendarTitle->FontSize(42);
    calendarTitle->TextColor({0.95f, 0.95f, 0.97f, 1.0f});
    calendarTitle->HorizontalGrow();
    calendarTitle->VerticalFixed(72);
    rightPanel->AddChild(std::move(calendarTitle));

    auto calendarDate = std::make_unique<UIElement>();
    calendarDate->Padding(8, 0, 20, 0);
    calendarDate->Text("Friday, December 19");
    calendarDate->FontSize(34);
    calendarDate->TextColor({0.5f, 0.5f, 0.55f, 1.0f});
    calendarDate->HorizontalGrow();
    calendarDate->VerticalFixed(60);
    rightPanel->AddChild(std::move(calendarDate));

    // Meeting Items
    struct Meeting {
        const char* time;
        const char* title;
        const char* duration;
        vec4 color;
    };

    Meeting meetings[] = {
        {"9:00 AM", "Team Standup", "30 min", {0.4f, 0.8f, 0.6f, 1.0f}},
        {"11:00 AM", "Design Review", "1 hour", {0.5f, 0.6f, 1.0f, 1.0f}},
        {"2:00 PM", "Client Call", "45 min", {0.9f, 0.5f, 0.8f, 1.0f}},
        {"4:30 PM", "Sprint Planning", "1.5 hours", {1.0f, 0.6f, 0.4f, 1.0f}}
    };

    for (int i = 0; i < 4; i++) {
        auto meetingCard = std::make_unique<UIElement>();
        meetingCard->MarginTop(16);
        meetingCard->Padding(24);
        meetingCard->Background({0.14f, 0.14f, 0.16f, 1.0f});
        meetingCard->BorderRadius({14, 14, 14, 14});
        meetingCard->BorderWidth({4, 0, 0, 0});
        meetingCard->BorderColor(meetings[i].color);
        meetingCard->VerticalStack();
        meetingCard->HorizontalGrow();
        meetingCard->VerticalFixed(160);

        auto meetingTime = std::make_unique<UIElement>();
        meetingTime->Padding(4, 0, 6, 0);
        meetingTime->Text(meetings[i].time);
        meetingTime->FontSize(32);
        meetingTime->TextColor(meetings[i].color);
        meetingTime->HorizontalGrow();
        meetingTime->VerticalFixed(48);
        meetingCard->AddChild(std::move(meetingTime));

        auto meetingTitle = std::make_unique<UIElement>();
        meetingTitle->Padding(4, 0, 6, 0);
        meetingTitle->Text(meetings[i].title);
        meetingTitle->FontSize(40);
        meetingTitle->TextColor({0.9f, 0.9f, 0.92f, 1.0f});
        meetingTitle->HorizontalGrow();
        meetingTitle->VerticalFixed(60);
        meetingCard->AddChild(std::move(meetingTitle));

        auto meetingDuration = std::make_unique<UIElement>();
        meetingDuration->Padding(4, 0, 4, 0);
        meetingDuration->Text(meetings[i].duration);
        meetingDuration->FontSize(30);
        meetingDuration->TextColor({0.5f, 0.5f, 0.55f, 1.0f});
        meetingDuration->HorizontalGrow();
        meetingDuration->VerticalGrow();
        meetingCard->AddChild(std::move(meetingDuration));

        rightPanel->AddChild(std::move(meetingCard));
    }

    // Spacer
    auto panelSpacer = std::make_unique<UIElement>();
    panelSpacer->VerticalGrow();
    rightPanel->AddChild(std::move(panelSpacer));

    // Team Online Section
    auto teamTitle = std::make_unique<UIElement>();
    teamTitle->Padding(8, 0, 16, 0);
    teamTitle->Text("Team Online");
    teamTitle->FontSize(38);
    teamTitle->TextColor({0.95f, 0.95f, 0.97f, 1.0f});
    teamTitle->HorizontalGrow();
    teamTitle->VerticalFixed(68);
    rightPanel->AddChild(std::move(teamTitle));

    auto teamAvatars = std::make_unique<UIElement>();
    teamAvatars->HorizontalStack();
    teamAvatars->HorizontalGrow();
    teamAvatars->VerticalFixed(64);

    vec4 teamColors[] = {
        {0.9f, 0.5f, 0.6f, 1.0f},
        {0.4f, 0.7f, 0.9f, 1.0f},
        {0.6f, 0.9f, 0.5f, 1.0f},
        {0.9f, 0.7f, 0.4f, 1.0f},
        {0.7f, 0.5f, 0.9f, 1.0f}
    };

    for (int i = 0; i < 5; i++) {
        auto teamAvatar = std::make_unique<UIElement>();
        if (i > 0) teamAvatar->MarginLeft(-14);
        teamAvatar->Background(teamColors[i]);
        teamAvatar->BorderRadius({26, 26, 26, 26});
        teamAvatar->BorderWidth({4, 4, 4, 4});
        teamAvatar->BorderColor({0.11f, 0.11f, 0.12f, 1.0f});
        teamAvatar->HorizontalFixed(52);
        teamAvatar->VerticalFixed(52);
        teamAvatars->AddChild(std::move(teamAvatar));
    }

    auto moreTeam = std::make_unique<UIElement>();
    moreTeam->MarginLeft(-14);
    moreTeam->Padding(12, 16, 12, 16);
    moreTeam->Background({0.2f, 0.2f, 0.22f, 1.0f});
    moreTeam->BorderRadius({26, 26, 26, 26});
    moreTeam->BorderWidth({4, 4, 4, 4});
    moreTeam->BorderColor({0.11f, 0.11f, 0.12f, 1.0f});
    moreTeam->Text("+8");
    moreTeam->FontSize(28);
    moreTeam->TextColor({0.6f, 0.6f, 0.65f, 1.0f});
    moreTeam->HorizontalFixed(52);
    moreTeam->VerticalFixed(52);
    teamAvatars->AddChild(std::move(moreTeam));

    rightPanel->AddChild(std::move(teamAvatars));

    mainContent->AddChild(std::move(rightPanel));
    root->AddChild(std::move(mainContent));
}

    auto Window::TestUI() -> void {
    root = std::make_unique<UIElement>();
    root->Padding(40);
    root->Background({0.08f, 0.08f, 0.09f, 1.0f});
    root->VerticalStack();
    root->HorizontalFixed(2800);
    root->VerticalFixed(2400);

    // ========== HEADER ==========
    auto header = std::make_unique<UIElement>();
    header->Padding(20);
    header->Background({0.15f, 0.15f, 0.17f, 1.0f});
    header->BorderRadius({12, 12, 12, 12});
    header->Text("TEXT LAYOUT DEBUG TEST");
    header->FontSize(48);
    header->TextColor({1.0f, 0.8f, 0.2f, 1.0f});
    header->HorizontalGrow();
    header->VerticalFixed(100);
    root->AddChild(std::move(header));

    // ========== SECTION 1: FONT SIZE VS CONTAINER HEIGHT ==========
    auto section1 = std::make_unique<UIElement>();
    section1->MarginTop(30);
    section1->Padding(24);
    section1->Background({0.12f, 0.12f, 0.13f, 1.0f});
    section1->BorderRadius({16, 16, 16, 16});
    section1->VerticalStack();
    section1->HorizontalGrow();
    section1->VerticalFixed(500);

    auto section1Title = std::make_unique<UIElement>();
    section1Title->Padding(8);
    section1Title->Text("TEST 1: Font Size vs Container Height (No Wrap)");
    section1Title->FontSize(36);
    section1Title->TextColor({0.5f, 0.8f, 1.0f, 1.0f});
    section1Title->HorizontalGrow();
    section1Title->VerticalFixed(60);
    section1->AddChild(std::move(section1Title));

    // Test rows: FontSize / Container Height combinations
    int fontSizes[] = {24, 32, 40, 48, 56};
    int containerHeights[] = {30, 40, 60, 80, 100};

    for (int row = 0; row < 5; row++) {
        auto testRow = std::make_unique<UIElement>();
        testRow->MarginTop(12);
        testRow->HorizontalStack();
        testRow->HorizontalGrow();
        testRow->VerticalFixed(containerHeights[row]);

        // Label
        auto label = std::make_unique<UIElement>();
        label->Padding(8);
        label->Background({0.2f, 0.2f, 0.22f, 1.0f});
        label->BorderRadius({6, 6, 6, 6});
        label->Text(("H:" + std::to_string(containerHeights[row])).c_str());
        label->FontSize(24);
        label->TextColor({0.7f, 0.7f, 0.7f, 1.0f});
        label->TextWrap(Graphic::GL::TextWrap::None);
        label->HorizontalFixed(80);
        label->VerticalGrow();
        testRow->AddChild(std::move(label));

        for (int col = 0; col < 5; col++) {
            auto testBox = std::make_unique<UIElement>();
            testBox->MarginLeft(12);
            testBox->Padding(4);
            testBox->Background({0.18f, 0.18f, 0.20f, 1.0f});
            testBox->BorderRadius({8, 8, 8, 8});
            testBox->BorderWidth({2, 2, 2, 2});

            // Color code: green if fontSize <= containerHeight, red if overflow likely
            if (fontSizes[col] <= containerHeights[row]) {
                testBox->BorderColor({0.3f, 0.8f, 0.4f, 1.0f});
            } else {
                testBox->BorderColor({1.0f, 0.4f, 0.3f, 1.0f});
            }

            testBox->Text(("F" + std::to_string(fontSizes[col])).c_str());
            testBox->FontSize(fontSizes[col]);
            testBox->TextColor({0.9f, 0.9f, 0.9f, 1.0f});
            testBox->TextWrap(Graphic::GL::TextWrap::None);
            testBox->HorizontalFixed(160);
            testBox->VerticalGrow();
            testRow->AddChild(std::move(testBox));
        }

        section1->AddChild(std::move(testRow));
    }

    root->AddChild(std::move(section1));

    // ========== SECTION 2: TEXT WRAP MODES ==========
    auto section2 = std::make_unique<UIElement>();
    section2->MarginTop(30);
    section2->Padding(24);
    section2->Background({0.12f, 0.12f, 0.13f, 1.0f});
    section2->BorderRadius({16, 16, 16, 16});
    section2->VerticalStack();
    section2->HorizontalGrow();
    section2->VerticalFixed(450);

    auto section2Title = std::make_unique<UIElement>();
    section2Title->Padding(8);
    section2Title->Text("TEST 2: Text Wrap Modes Comparison");
    section2Title->FontSize(36);
    section2Title->TextColor({0.5f, 0.8f, 1.0f, 1.0f});
    section2Title->HorizontalGrow();
    section2Title->VerticalFixed(60);
    section2->AddChild(std::move(section2Title));

    auto wrapRow = std::make_unique<UIElement>();
    wrapRow->MarginTop(16);
    wrapRow->HorizontalStack();
    wrapRow->HorizontalGrow();
    wrapRow->VerticalGrow();

    // None wrap test
    auto wrapNoneContainer = std::make_unique<UIElement>();
    wrapNoneContainer->Padding(16);
    wrapNoneContainer->Background({0.16f, 0.16f, 0.18f, 1.0f});
    wrapNoneContainer->BorderRadius({12, 12, 12, 12});
    wrapNoneContainer->BorderWidth({3, 3, 3, 3});
    wrapNoneContainer->BorderColor({1.0f, 0.5f, 0.3f, 1.0f});
    wrapNoneContainer->VerticalStack();
    wrapNoneContainer->HorizontalFixed(400);
    wrapNoneContainer->VerticalGrow();

    auto wrapNoneLabel = std::make_unique<UIElement>();
    wrapNoneLabel->Padding(8);
    wrapNoneLabel->Text("TextWrap::None");
    wrapNoneLabel->FontSize(32);
    wrapNoneLabel->TextColor({1.0f, 0.5f, 0.3f, 1.0f});
    wrapNoneLabel->HorizontalGrow();
    wrapNoneLabel->VerticalFixed(50);
    wrapNoneContainer->AddChild(std::move(wrapNoneLabel));

    auto wrapNoneText = std::make_unique<UIElement>();
    wrapNoneText->Padding(12);
    wrapNoneText->Background({0.1f, 0.1f, 0.11f, 1.0f});
    wrapNoneText->BorderRadius({8, 8, 8, 8});
    wrapNoneText->Text("This is a long text that should NOT wrap to the next line when using None mode");
    wrapNoneText->FontSize(36);
    wrapNoneText->TextColor({0.85f, 0.85f, 0.88f, 1.0f});
    wrapNoneText->TextWrap(Graphic::GL::TextWrap::None);
    wrapNoneText->HorizontalGrow();
    wrapNoneText->VerticalGrow();
    wrapNoneContainer->AddChild(std::move(wrapNoneText));

    wrapRow->AddChild(std::move(wrapNoneContainer));

    // Character wrap test
    auto wrapCharContainer = std::make_unique<UIElement>();
    wrapCharContainer->MarginLeft(24);
    wrapCharContainer->Padding(16);
    wrapCharContainer->Background({0.16f, 0.16f, 0.18f, 1.0f});
    wrapCharContainer->BorderRadius({12, 12, 12, 12});
    wrapCharContainer->BorderWidth({3, 3, 3, 3});
    wrapCharContainer->BorderColor({0.3f, 0.8f, 0.5f, 1.0f});
    wrapCharContainer->VerticalStack();
    wrapCharContainer->HorizontalFixed(400);
    wrapCharContainer->VerticalGrow();

    auto wrapCharLabel = std::make_unique<UIElement>();
    wrapCharLabel->Padding(8);
    wrapCharLabel->Text("TextWrap::Character");
    wrapCharLabel->FontSize(32);
    wrapCharLabel->TextColor({0.3f, 0.8f, 0.5f, 1.0f});
    wrapCharLabel->HorizontalGrow();
    wrapCharLabel->VerticalFixed(50);
    wrapCharContainer->AddChild(std::move(wrapCharLabel));

    auto wrapCharText = std::make_unique<UIElement>();
    wrapCharText->Padding(12);
    wrapCharText->Background({0.1f, 0.1f, 0.11f, 1.0f});
    wrapCharText->BorderRadius({8, 8, 8, 8});
    wrapCharText->Text("This is a long text that SHOULD wrap to the next line when using Character mode");
    wrapCharText->FontSize(36);
    wrapCharText->TextColor({0.85f, 0.85f, 0.88f, 1.0f});
    wrapCharText->TextWrap(Graphic::GL::TextWrap::Character);
    wrapCharText->HorizontalGrow();
    wrapCharText->VerticalGrow();
    wrapCharContainer->AddChild(std::move(wrapCharText));

    wrapRow->AddChild(std::move(wrapCharContainer));

    // Side by side same width different content
    auto wrapCompareContainer = std::make_unique<UIElement>();
    wrapCompareContainer->MarginLeft(24);
    wrapCompareContainer->Padding(16);
    wrapCompareContainer->Background({0.16f, 0.16f, 0.18f, 1.0f});
    wrapCompareContainer->BorderRadius({12, 12, 12, 12});
    wrapCompareContainer->BorderWidth({3, 3, 3, 3});
    wrapCompareContainer->BorderColor({0.8f, 0.6f, 1.0f, 1.0f});
    wrapCompareContainer->VerticalStack();
    wrapCompareContainer->HorizontalGrow();
    wrapCompareContainer->VerticalGrow();

    auto wrapCompareLabel = std::make_unique<UIElement>();
    wrapCompareLabel->Padding(8);
    wrapCompareLabel->Text("Narrow Container (200px) + Character Wrap");
    wrapCompareLabel->FontSize(32);
    wrapCompareLabel->TextColor({0.8f, 0.6f, 1.0f, 1.0f});
    wrapCompareLabel->HorizontalGrow();
    wrapCompareLabel->VerticalFixed(50);
    wrapCompareContainer->AddChild(std::move(wrapCompareLabel));

    auto narrowWrapText = std::make_unique<UIElement>();
    narrowWrapText->Padding(12);
    narrowWrapText->Background({0.1f, 0.1f, 0.11f, 1.0f});
    narrowWrapText->BorderRadius({8, 8, 8, 8});
    narrowWrapText->Text("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    narrowWrapText->FontSize(40);
    narrowWrapText->TextColor({0.85f, 0.85f, 0.88f, 1.0f});
    narrowWrapText->TextWrap(Graphic::GL::TextWrap::Character);
    narrowWrapText->HorizontalFixed(200);
    narrowWrapText->VerticalGrow();
    wrapCompareContainer->AddChild(std::move(narrowWrapText));

    wrapRow->AddChild(std::move(wrapCompareContainer));

    section2->AddChild(std::move(wrapRow));
    root->AddChild(std::move(section2));

    // ========== SECTION 3: HORIZONTAL SIZING MODES ==========
    auto section3 = std::make_unique<UIElement>();
    section3->MarginTop(30);
    section3->Padding(24);
    section3->Background({0.12f, 0.12f, 0.13f, 1.0f});
    section3->BorderRadius({16, 16, 16, 16});
    section3->VerticalStack();
    section3->HorizontalGrow();
    section3->VerticalFixed(350);

    auto section3Title = std::make_unique<UIElement>();
    section3Title->Padding(8);
    section3Title->Text("TEST 3: Horizontal Sizing Modes with Text");
    section3Title->FontSize(36);
    section3Title->TextColor({0.5f, 0.8f, 1.0f, 1.0f});
    section3Title->HorizontalGrow();
    section3Title->VerticalFixed(60);
    section3->AddChild(std::move(section3Title));

    // HorizontalFixed
    auto hFixedRow = std::make_unique<UIElement>();
    hFixedRow->MarginTop(12);
    hFixedRow->HorizontalStack();
    hFixedRow->HorizontalGrow();
    hFixedRow->VerticalFixed(80);

    auto hFixedLabel = std::make_unique<UIElement>();
    hFixedLabel->Padding(12);
    hFixedLabel->Background({0.2f, 0.2f, 0.22f, 1.0f});
    hFixedLabel->BorderRadius({8, 8, 8, 8});
    hFixedLabel->Text("HorizontalFixed(300):");
    hFixedLabel->FontSize(32);
    hFixedLabel->TextColor({0.7f, 0.7f, 0.75f, 1.0f});
    hFixedLabel->HorizontalFixed(320);
    hFixedLabel->VerticalGrow();
    hFixedRow->AddChild(std::move(hFixedLabel));

    auto hFixedBox = std::make_unique<UIElement>();
    hFixedBox->MarginLeft(16);
    hFixedBox->Padding(12);
    hFixedBox->Background({0.25f, 0.15f, 0.15f, 0.2f});
    hFixedBox->BorderRadius({8, 8, 8, 8});
    hFixedBox->BorderWidth({2, 2, 2, 2});
    hFixedBox->BorderColor({1.0f, 0.4f, 0.4f, 1.0f});
    hFixedBox->Text("Fixed width text box");
    hFixedBox->FontSize(40);
    hFixedBox->TextColor({0.9f, 0.9f, 0.9f, 1.0f});
    hFixedBox->TextWrap(Graphic::GL::TextWrap::None);
    hFixedBox->HorizontalFixed(300);
    hFixedBox->VerticalGrow();
    hFixedRow->AddChild(std::move(hFixedBox));

    section3->AddChild(std::move(hFixedRow));

    // HorizontalGrow
    auto hGrowRow = std::make_unique<UIElement>();
    hGrowRow->MarginTop(12);
    hGrowRow->HorizontalStack();
    hGrowRow->HorizontalGrow();
    hGrowRow->VerticalFixed(80);

    auto hGrowLabel = std::make_unique<UIElement>();
    hGrowLabel->Padding(12);
    hGrowLabel->Background({0.2f, 0.2f, 0.22f, 1.0f});
    hGrowLabel->BorderRadius({8, 8, 8, 8});
    hGrowLabel->Text("HorizontalGrow():");
    hGrowLabel->FontSize(32);
    hGrowLabel->TextColor({0.7f, 0.7f, 0.75f, 1.0f});
    hGrowLabel->HorizontalFixed(320);
    hGrowLabel->VerticalGrow();
    hGrowRow->AddChild(std::move(hGrowLabel));

    auto hGrowBox = std::make_unique<UIElement>();
    hGrowBox->MarginLeft(16);
    hGrowBox->Padding(12);
    hGrowBox->Background({0.15f, 0.25f, 0.15f, 1.0f});
    hGrowBox->BorderRadius({8, 8, 8, 8});
    hGrowBox->BorderWidth({2, 2, 2, 2});
    hGrowBox->BorderColor({0.4f, 1.0f, 0.4f, 1.0f});
    hGrowBox->Text("Growing width text box - should expand");
    hGrowBox->FontSize(40);
    hGrowBox->TextColor({0.9f, 0.9f, 0.9f, 1.0f});
    hGrowBox->TextWrap(Graphic::GL::TextWrap::None);
    hGrowBox->HorizontalGrow();
    hGrowBox->VerticalGrow();
    hGrowRow->AddChild(std::move(hGrowBox));

    section3->AddChild(std::move(hGrowRow));

    // HorizontalFit
    auto hFitRow = std::make_unique<UIElement>();
    hFitRow->MarginTop(12);
    hFitRow->HorizontalStack();
    hFitRow->HorizontalGrow();
    hFitRow->VerticalFixed(80);

    auto hFitLabel = std::make_unique<UIElement>();
    hFitLabel->Padding(12);
    hFitLabel->Background({0.2f, 0.2f, 0.22f, 1.0f});
    hFitLabel->BorderRadius({8, 8, 8, 8});
    hFitLabel->Text("HorizontalFit():");
    hFitLabel->FontSize(32);
    hFitLabel->TextColor({0.7f, 0.7f, 0.75f, 1.0f});
    hFitLabel->HorizontalFixed(320);
    hFitLabel->VerticalGrow();
    hFitRow->AddChild(std::move(hFitLabel));

    auto hFitBox = std::make_unique<UIElement>();
    hFitBox->MarginLeft(16);
    hFitBox->Padding(12);
    hFitBox->Background({0.15f, 0.15f, 0.25f, 1.0f});
    hFitBox->BorderRadius({8, 8, 8, 8});
    hFitBox->BorderWidth({2, 2, 2, 2});
    hFitBox->BorderColor({0.4f, 0.4f, 1.0f, 1.0f});
    hFitBox->Text("Fit to content");
    hFitBox->FontSize(40);
    hFitBox->TextColor({0.9f, 0.9f, 0.9f, 1.0f});
    hFitBox->TextWrap(Graphic::GL::TextWrap::None);
    hFitBox->HorizontalFit();
    hFitBox->VerticalGrow();
    hFitRow->AddChild(std::move(hFitBox));

    auto hFitBox2 = std::make_unique<UIElement>();
    hFitBox2->MarginLeft(16);
    hFitBox2->Padding(12);
    hFitBox2->Background({0.15f, 0.15f, 0.25f, 1.0f});
    hFitBox2->BorderRadius({8, 8, 8, 8});
    hFitBox2->BorderWidth({2, 2, 2, 2});
    hFitBox2->BorderColor({0.4f, 0.4f, 1.0f, 1.0f});
    hFitBox2->Text("Short");
    hFitBox2->FontSize(40);
    hFitBox2->TextColor({0.9f, 0.9f, 0.9f, 1.0f});
    hFitBox2->TextWrap(Graphic::GL::TextWrap::None);
    hFitBox2->HorizontalFit();
    hFitBox2->VerticalGrow();
    hFitRow->AddChild(std::move(hFitBox2));

    auto hFitBox3 = std::make_unique<UIElement>();
    hFitBox3->MarginLeft(16);
    hFitBox3->Padding(12);
    hFitBox3->Background({0.15f, 0.15f, 0.25f, 1.0f});
    hFitBox3->BorderRadius({8, 8, 8, 8});
    hFitBox3->BorderWidth({2, 2, 2, 2});
    hFitBox3->BorderColor({0.4f, 0.4f, 1.0f, 1.0f});
    hFitBox3->Text("A much longer piece of text here");
    hFitBox3->FontSize(40);
    hFitBox3->TextColor({0.9f, 0.9f, 0.9f, 1.0f});
    hFitBox3->TextWrap(Graphic::GL::TextWrap::None);
    hFitBox3->HorizontalFit();
    hFitBox3->VerticalGrow();
    hFitRow->AddChild(std::move(hFitBox3));

    section3->AddChild(std::move(hFitRow));
    root->AddChild(std::move(section3));

    // ========== SECTION 4: VERTICAL SIZING WITH WRAPPED TEXT ==========
    auto section4 = std::make_unique<UIElement>();
    section4->MarginTop(30);
    section4->Padding(24);
    section4->Background({0.12f, 0.12f, 0.13f, 1.0f});
    section4->BorderRadius({16, 16, 16, 16});
    section4->VerticalStack();
    section4->HorizontalGrow();
    section4->VerticalFixed(400);

    auto section4Title = std::make_unique<UIElement>();
    section4Title->Padding(8);
    section4Title->Text("TEST 4: Vertical Sizing with Wrapped Text");
    section4Title->FontSize(36);
    section4Title->TextColor({0.5f, 0.8f, 1.0f, 1.0f});
    section4Title->HorizontalGrow();
    section4Title->VerticalFixed(60);
    section4->AddChild(std::move(section4Title));

    auto vTestRow = std::make_unique<UIElement>();
    vTestRow->MarginTop(16);
    vTestRow->HorizontalStack();
    vTestRow->HorizontalGrow();
    vTestRow->VerticalGrow();

    // VerticalFixed with Character wrap
    auto vFixedWrap = std::make_unique<UIElement>();
    vFixedWrap->Padding(16);
    vFixedWrap->Background({0.18f, 0.18f, 0.20f, 1.0f});
    vFixedWrap->BorderRadius({12, 12, 12, 12});
    vFixedWrap->BorderWidth({2, 2, 2, 2});
    vFixedWrap->BorderColor({1.0f, 0.6f, 0.2f, 1.0f});
    vFixedWrap->VerticalStack();
    vFixedWrap->HorizontalFixed(350);
    vFixedWrap->VerticalGrow();

    auto vFixedLabel = std::make_unique<UIElement>();
    vFixedLabel->Padding(8);
    vFixedLabel->Text("VerticalFixed(100) + Wrap");
    vFixedLabel->FontSize(28);
    vFixedLabel->TextColor({1.0f, 0.6f, 0.2f, 1.0f});
    vFixedLabel->HorizontalGrow();
    vFixedLabel->VerticalFixed(44);
    vFixedWrap->AddChild(std::move(vFixedLabel));

    auto vFixedText = std::make_unique<UIElement>();
    vFixedText->Padding(12);
    vFixedText->Background({0.1f, 0.1f, 0.11f, 1.0f});
    vFixedText->BorderRadius({8, 8, 8, 8});
    vFixedText->Text("This text should wrap and be clipped at 100px height. Lorem ipsum dolor sit amet.");
    vFixedText->FontSize(36);
    vFixedText->TextColor({0.85f, 0.85f, 0.88f, 1.0f});
    vFixedText->TextWrap(Graphic::GL::TextWrap::Character);
    vFixedText->HorizontalGrow();
    vFixedText->VerticalFixed(100);
    vFixedWrap->AddChild(std::move(vFixedText));

    vTestRow->AddChild(std::move(vFixedWrap));

    // VerticalGrow with Character wrap
    auto vGrowWrap = std::make_unique<UIElement>();
    vGrowWrap->MarginLeft(24);
    vGrowWrap->Padding(16);
    vGrowWrap->Background({0.18f, 0.18f, 0.20f, 1.0f});
    vGrowWrap->BorderRadius({12, 12, 12, 12});
    vGrowWrap->BorderWidth({2, 2, 2, 2});
    vGrowWrap->BorderColor({0.2f, 0.8f, 0.6f, 1.0f});
    vGrowWrap->VerticalStack();
    vGrowWrap->HorizontalFixed(350);
    vGrowWrap->VerticalGrow();

    auto vGrowLabel = std::make_unique<UIElement>();
    vGrowLabel->Padding(8);
    vGrowLabel->Text("VerticalGrow() + Wrap");
    vGrowLabel->FontSize(28);
    vGrowLabel->TextColor({0.2f, 0.8f, 0.6f, 1.0f});
    vGrowLabel->HorizontalGrow();
    vGrowLabel->VerticalFixed(44);
    vGrowWrap->AddChild(std::move(vGrowLabel));

    auto vGrowText = std::make_unique<UIElement>();
    vGrowText->Padding(12);
    vGrowText->Background({0.1f, 0.1f, 0.11f, 1.0f});
    vGrowText->BorderRadius({8, 8, 8, 8});
    vGrowText->Text("This text should wrap and grow to fill available space. The container should expand.");
    vGrowText->FontSize(36);
    vGrowText->TextColor({0.85f, 0.85f, 0.88f, 1.0f});
    vGrowText->TextWrap(Graphic::GL::TextWrap::Character);
    vGrowText->HorizontalGrow();
    vGrowText->VerticalGrow();
    vGrowWrap->AddChild(std::move(vGrowText));

    vTestRow->AddChild(std::move(vGrowWrap));

    // VerticalFit with Character wrap (if supported)
    auto vFitWrap = std::make_unique<UIElement>();
    vFitWrap->MarginLeft(24);
    vFitWrap->Padding(16);
    vFitWrap->Background({0.18f, 0.18f, 0.20f, 1.0f});
    vFitWrap->BorderRadius({12, 12, 12, 12});
    vFitWrap->BorderWidth({2, 2, 2, 2});
    vFitWrap->BorderColor({0.6f, 0.4f, 1.0f, 1.0f});
    vFitWrap->VerticalStack();
    vFitWrap->HorizontalFixed(350);
    vFitWrap->VerticalGrow();

    auto vFitLabel = std::make_unique<UIElement>();
    vFitLabel->Padding(8);
    vFitLabel->Text("VerticalFit() + Wrap");
    vFitLabel->FontSize(28);
    vFitLabel->TextColor({0.6f, 0.4f, 1.0f, 1.0f});
    vFitLabel->HorizontalGrow();
    vFitLabel->VerticalFixed(44);
    vFitWrap->AddChild(std::move(vFitLabel));

    auto vFitText = std::make_unique<UIElement>();
    vFitText->Padding(12);
    vFitText->Background({0.1f, 0.1f, 0.11f, 1.0f});
    vFitText->BorderRadius({8, 8, 8, 8});
    vFitText->Text("VerticalFit should size to wrapped content height if supported.");
    vFitText->FontSize(36);
    vFitText->TextColor({0.85f, 0.85f, 0.88f, 1.0f});
    vFitText->TextWrap(Graphic::GL::TextWrap::Character);
    vFitText->HorizontalGrow();
    vFitText->VerticalFit();
    vFitWrap->AddChild(std::move(vFitText));

    vTestRow->AddChild(std::move(vFitWrap));

    section4->AddChild(std::move(vTestRow));
    root->AddChild(std::move(section4));

    // ========== SECTION 5: EDGE CASES ==========
    auto section5 = std::make_unique<UIElement>();
    section5->MarginTop(30);
    section5->Padding(24);
    section5->Background({0.12f, 0.12f, 0.13f, 1.0f});
    section5->BorderRadius({16, 16, 16, 16});
    section5->VerticalStack();
    section5->HorizontalGrow();
    section5->VerticalFixed(350);

    auto section5Title = std::make_unique<UIElement>();
    section5Title->Padding(8);
    section5Title->Text("TEST 5: Edge Cases");
    section5Title->FontSize(36);
    section5Title->TextColor({0.5f, 0.8f, 1.0f, 1.0f});
    section5Title->HorizontalGrow();
    section5Title->VerticalFixed(60);
    section5->AddChild(std::move(section5Title));

    auto edgeRow = std::make_unique<UIElement>();
    edgeRow->MarginTop(16);
    edgeRow->HorizontalStack();
    edgeRow->HorizontalGrow();
    edgeRow->VerticalGrow();

    // Empty text
    auto emptyText = std::make_unique<UIElement>();
    emptyText->Padding(16);
    emptyText->Background({0.18f, 0.18f, 0.20f, 1.0f});
    emptyText->BorderRadius({12, 12, 12, 12});
    emptyText->BorderWidth({2, 2, 2, 2});
    emptyText->BorderColor({0.5f, 0.5f, 0.5f, 1.0f});
    emptyText->VerticalStack();
    emptyText->HorizontalFixed(250);
    emptyText->VerticalGrow();

    auto emptyLabel = std::make_unique<UIElement>();
    emptyLabel->Padding(8);
    emptyLabel->Text("Empty String");
    emptyLabel->FontSize(28);
    emptyLabel->TextColor({0.7f, 0.7f, 0.7f, 1.0f});
    emptyLabel->HorizontalGrow();
    emptyLabel->VerticalFixed(44);
    emptyText->AddChild(std::move(emptyLabel));

    auto emptyBox = std::make_unique<UIElement>();
    emptyBox->Padding(12);
    emptyBox->Background({0.1f, 0.1f, 0.11f, 1.0f});
    emptyBox->BorderRadius({8, 8, 8, 8});
    emptyBox->Text("");
    emptyBox->FontSize(40);
    emptyBox->HorizontalGrow();
    emptyBox->VerticalFixed(80);
    emptyText->AddChild(std::move(emptyBox));

    edgeRow->AddChild(std::move(emptyText));

    // Single character
    auto singleChar = std::make_unique<UIElement>();
    singleChar->MarginLeft(24);
    singleChar->Padding(16);
    singleChar->Background({0.18f, 0.18f, 0.20f, 1.0f});
    singleChar->BorderRadius({12, 12, 12, 12});
    singleChar->BorderWidth({2, 2, 2, 2});
    singleChar->BorderColor({0.5f, 0.5f, 0.5f, 1.0f});
    singleChar->VerticalStack();
    singleChar->HorizontalFixed(250);
    singleChar->VerticalGrow();

    auto singleLabel = std::make_unique<UIElement>();
    singleLabel->Padding(8);
    singleLabel->Text("Single Char");
    singleLabel->FontSize(28);
    singleLabel->TextColor({0.7f, 0.7f, 0.7f, 1.0f});
    singleLabel->HorizontalGrow();
    singleLabel->VerticalFixed(44);
    singleChar->AddChild(std::move(singleLabel));

    auto singleBox = std::make_unique<UIElement>();
    singleBox->Padding(12);
    singleBox->Background({0.1f, 0.1f, 0.11f, 1.0f});
    singleBox->BorderRadius({8, 8, 8, 8});
    singleBox->Text("X");
    singleBox->FontSize(40);
    singleBox->TextColor({0.9f, 0.9f, 0.9f, 1.0f});
    singleBox->HorizontalGrow();
    singleBox->VerticalFixed(80);
    singleChar->AddChild(std::move(singleBox));

    edgeRow->AddChild(std::move(singleChar));

    // Very long word (no spaces)
    auto longWord = std::make_unique<UIElement>();
    longWord->MarginLeft(24);
    longWord->Padding(16);
    longWord->Background({0.18f, 0.18f, 0.20f, 1.0f});
    longWord->BorderRadius({12, 12, 12, 12});
    longWord->BorderWidth({2, 2, 2, 2});
    longWord->BorderColor({1.0f, 0.4f, 0.4f, 1.0f});
    longWord->VerticalStack();
    longWord->HorizontalFixed(250);
    longWord->VerticalGrow();

    auto longLabel = std::make_unique<UIElement>();
    longLabel->Padding(8);
    longLabel->Text("Long Word + Wrap");
    longLabel->FontSize(28);
    longLabel->TextColor({1.0f, 0.4f, 0.4f, 1.0f});
    longLabel->HorizontalGrow();
    longLabel->VerticalFixed(44);
    longWord->AddChild(std::move(longLabel));

    auto longBox = std::make_unique<UIElement>();
    longBox->Padding(12);
    longBox->Background({0.1f, 0.1f, 0.11f, 1.0f});
    longBox->BorderRadius({8, 8, 8, 8});
    longBox->Text("Supercalifragilisticexpialidocious");
    longBox->FontSize(36);
    longBox->TextColor({0.9f, 0.9f, 0.9f, 1.0f});
    longBox->TextWrap(Graphic::GL::TextWrap::Character);
    longBox->HorizontalGrow();
    longBox->VerticalGrow();
    longWord->AddChild(std::move(longBox));

    edgeRow->AddChild(std::move(longWord));

    // Special characters
    auto specialChars = std::make_unique<UIElement>();
    specialChars->MarginLeft(24);
    specialChars->Padding(16);
    specialChars->Background({0.18f, 0.18f, 0.20f, 1.0f});
    specialChars->BorderRadius({12, 12, 12, 12});
    specialChars->BorderWidth({2, 2, 2, 2});
    specialChars->BorderColor({0.5f, 0.5f, 0.5f, 1.0f});
    specialChars->VerticalStack();
    specialChars->HorizontalFixed(250);
    specialChars->VerticalGrow();

    auto specialLabel = std::make_unique<UIElement>();
    specialLabel->Padding(8);
    specialLabel->Text("Special Chars");
    specialLabel->FontSize(28);
    specialLabel->TextColor({0.7f, 0.7f, 0.7f, 1.0f});
    specialLabel->HorizontalGrow();
    specialLabel->VerticalFixed(44);
    specialChars->AddChild(std::move(specialLabel));

    auto specialBox = std::make_unique<UIElement>();
    specialBox->Padding(12);
    specialBox->Background({0.1f, 0.1f, 0.11f, 1.0f});
    specialBox->BorderRadius({8, 8, 8, 8});
    specialBox->Text("!@#$%^&*()_+-=[]{}|;':\",./<>?");
    specialBox->FontSize(32);
    specialBox->TextColor({0.9f, 0.9f, 0.9f, 1.0f});
    specialBox->TextWrap(Graphic::GL::TextWrap::Character);
    specialBox->HorizontalGrow();
    specialBox->VerticalGrow();
    specialChars->AddChild(std::move(specialBox));

    edgeRow->AddChild(std::move(specialChars));

    // Numbers only
    auto numbersOnly = std::make_unique<UIElement>();
    numbersOnly->MarginLeft(24);
    numbersOnly->Padding(16);
    numbersOnly->Background({0.18f, 0.18f, 0.20f, 1.0f});
    numbersOnly->BorderRadius({12, 12, 12, 12});
    numbersOnly->BorderWidth({2, 2, 2, 2});
    numbersOnly->BorderColor({0.5f, 0.5f, 0.5f, 1.0f});
    numbersOnly->VerticalStack();
    numbersOnly->HorizontalGrow();
    numbersOnly->VerticalGrow();

    auto numbersLabel = std::make_unique<UIElement>();
    numbersLabel->Padding(8);
    numbersLabel->Text("Numbers / Newlines");
    numbersLabel->FontSize(28);
    numbersLabel->TextColor({0.7f, 0.7f, 0.7f, 1.0f});
    numbersLabel->HorizontalGrow();
    numbersLabel->VerticalFixed(44);
    numbersOnly->AddChild(std::move(numbersLabel));

    auto numbersBox = std::make_unique<UIElement>();
    numbersBox->Padding(12);
    numbersBox->Background({0.1f, 0.1f, 0.11f, 1.0f});
    numbersBox->BorderRadius({8, 8, 8, 8});
    numbersBox->Text("Line1\nLine2\nLine3\n12345.67890");
    numbersBox->FontSize(36);
    numbersBox->TextColor({0.9f, 0.9f, 0.9f, 1.0f});
    numbersBox->TextWrap(Graphic::GL::TextWrap::Character);
    numbersBox->HorizontalGrow();
    numbersBox->VerticalGrow();
    numbersOnly->AddChild(std::move(numbersBox));

    edgeRow->AddChild(std::move(numbersOnly));

    section5->AddChild(std::move(edgeRow));
    root->AddChild(std::move(section5));
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
