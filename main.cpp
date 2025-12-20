#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <memory>

#include "core/AssetController.h"
#include "core/ObjectPoolManager.h"
#include "graphic/WindowInterface.h"
#include "graphic/gl/GLFWWindow.h"
#include "graphic/gl/Mesh.h"
#include "graphic/gl/SDLWindow.h"
#include "graphic/gl/Shader.h"
#include "graphic/gl/ShaderUtil.h"
#include "graphic/gl/TextRenderer.h"
#include "graphic/gl/UIRender.h"
#include "graphic/gl/WindowController.h"
#include "graphic/text/GlyphAtlas.h"
#include "UI/Window.h"

using namespace std;

// Mouse picking context
struct MousePickCtx {
    Funccia::Graphic::GL::Mesh* mesh = nullptr;
    float planeZ = 0.0f;
    int windowWidth = 1;
    int windowHeight = 1;
};

int main() {
    // Initialize core systems
    Funccia::Core::AssetController::Instance().Initialize(100 * 1024 * 1024);
    Funccia::Core::ObjectPoolManager::AssetPool = new Funccia::Core::ObjectPool<Funccia::Core::Asset>();

    // Create and initialize window properly
    auto window = std::make_unique<Funccia::Graphic::GL::SDLWindow>();

    if (!window->Initialize(1920, 1080, "FuncciaFrame")) {
        std::cerr << "Failed to initialize window!" << std::endl;
        return -1;
    }

    auto shader = Funccia::Graphic::GL::Shader();
    shader.initialize("/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/vertex/basic2-ver.glsl",
                     "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/fragment/basic2-fag.glsl");

    // Setup OpenGL state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    // Create dodecahedron mesh
    vector<float> vertices = {
        -0.26286500f,  0.00000000f,  0.42532500f,
         0.26286500f,  0.00000000f,  0.42532500f,
        -0.26286500f,  0.00000000f, -0.42532500f,
         0.26286500f,  0.00000000f, -0.42532500f,
         0.00000000f,  0.42532500f,  0.26286500f,
         0.00000000f,  0.42532500f, -0.26286500f,
         0.00000000f, -0.42532500f,  0.26286500f,
         0.00000000f, -0.42532500f, -0.26286500f,
         0.42532500f,  0.26286500f,  0.00000000f,
        -0.42532500f,  0.26286500f,  0.00000000f,
         0.42532500f, -0.26286500f,  0.00000000f,
        -0.42532500f, -0.26286500f,  0.00000000f
    };

    vector<int> indices = {
        0, 6, 1,   0,11, 6,   1, 4, 0,
        1, 8, 4,   1,10, 8,   2, 5, 3,
        2, 9, 5,   2,11, 9,   3, 7, 2,
        3,10, 7,   4, 8, 5,   4, 9, 0,
        5, 8, 3,   5, 9, 4,   6,10, 1,
        6,11, 7,   7,10, 6,   7,11, 2,
        8,10, 3,   9,11, 0
    };

    // Scale vertices
    for (auto& v : vertices) {
        v *= 1.2f;
    }

    auto mesh = Funccia::Graphic::GL::Mesh(vertices, indices);
    mesh.Create();

    Funccia::Graphic::GL::UIRenderer renderer = Funccia::Graphic::GL::UIRenderer();
    renderer.Initialize("/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/vertex/UIShader-ver.glsl",
        "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/fragment/UIShader-frag.glsl");
    Funccia::UI::Window uiWindow = Funccia::UI::Window();

    // Setup mouse picking context
    MousePickCtx ctx;
    ctx.mesh = &mesh;
    ctx.planeZ = 0.0f;

    // Mouse callback for picking (platform-independent approach using GetCursorPos)
    auto lastMouseX = 0.0;
    auto lastMouseY = 0.0;

    auto handleMousePicking = [&]() {
        double xpos, ypos;
        window->GetCursorPos(xpos, ypos);

        // Only process if mouse moved
        if (xpos == lastMouseX && ypos == lastMouseY) return;
        lastMouseX = xpos;
        lastMouseY = ypos;

        int winW, winH;
        window->GetFramebufferSize(winW, winH);

        float x_ndc = 2.0f * float(xpos) / float(winW) - 1.0f;
        float y_ndc = 1.0f - 2.0f * float(ypos) / float(winH);

        glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        float aspect = (float)viewport[2] / (float)viewport[3];
        glm::mat4 P = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
        glm::mat4 invPV = glm::inverse(P * V);

        glm::vec4 nearNDC(x_ndc, y_ndc, -1.0f, 1.0f);
        glm::vec4 farNDC(x_ndc, y_ndc, 1.0f, 1.0f);

        glm::vec4 nearW = invPV * nearNDC; nearW /= nearW.w;
        glm::vec4 farW = invPV * farNDC; farW /= farW.w;

        glm::vec3 rayOrigin = glm::vec3(nearW);
        glm::vec3 rayDir = glm::normalize(glm::vec3(farW - nearW));

        float denom = rayDir.z;
        if (std::abs(denom) < 1e-6f) return;

        float t = (ctx.planeZ - rayOrigin.z) / denom;
        if (t < 0.0f) return;

        glm::vec3 hit = rayOrigin + t * rayDir;
        ctx.mesh->GetTransform().setPositionXPlane({ hit.x, hit.y });
    };

    int winW, winH;
    window->GetFramebufferSize(winW, winH);

    renderer.Render(glm::mat4(1.0f), glm::vec2(winW, winH));

    // Timing variables
    auto lastTime = std::chrono::high_resolution_clock::now();
    int framesSinceUpdate = 0;
    auto fpsWindowStart = lastTime;
    char titleBuf[128];

#ifdef FF_UI_LAZY_LAYOUT
    uiWindow.InitLayout(0, 0, glm::vec2(winW, winH));
#endif


    auto textRender = std::make_unique<Funccia::Graphic::GL::TextRenderer>();
    textRender->Initialize("/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/vertex/FontShader1-ver.glsl",
        "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/fragment/FontShader1-fag.glsl");




    // Main render loop
    while (!window->ShouldClose()) {
        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;

        // Process input
        const float rotSpeed = 120.0f;
        const float moveSpeed = 3.0f;
        window->PollEvents();

        using Key = Funccia::Graphic::Key;

        // Platform-independent input
        if (window->IsKeyPressed(Key::Escape)) {
            window->SetShouldClose(true);
        }

        if (window->IsKeyPressed(Key::W)) mesh.Rotate(-rotSpeed * dt, 0, 0);
        if (window->IsKeyPressed(Key::S)) mesh.Rotate(rotSpeed * dt, 0, 0);
        if (window->IsKeyPressed(Key::A)) mesh.Rotate(0, -rotSpeed * dt, 0);
        if (window->IsKeyPressed(Key::D)) mesh.Rotate(0, rotSpeed * dt, 0);
        if (window->IsKeyPressed(Key::E)) mesh.Rotate(0, 0, -rotSpeed * dt);
        if (window->IsKeyPressed(Key::Q)) mesh.Rotate(0, 0, rotSpeed * dt);

        if (window->IsKeyPressed(Key::Up)) mesh.MoveUp(moveSpeed * dt);
        if (window->IsKeyPressed(Key::Down)) mesh.MoveUp(-moveSpeed * dt);
        if (window->IsKeyPressed(Key::Left)) mesh.MoveRight(-moveSpeed * dt);
        if (window->IsKeyPressed(Key::Right)) mesh.MoveRight(moveSpeed * dt);
        if (window->IsKeyPressed(Key::Space)) mesh.MoveForward(moveSpeed * dt);
        if (window->IsKeyPressed(Key::LeftShift)) mesh.MoveForward(-moveSpeed * dt);

        // Handle mouse picking (platform-independent)
        handleMousePicking();

        // Clear and render 3D scene
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mesh.Draw(&shader);

        window->GetFramebufferSize(winW, winH);
        glViewport(0, 0, winW, winH);
        // =================window ui===============================
        auto frameStart = std::chrono::high_resolution_clock::now();

        renderer.BeginFrame();
        textRender->BeginFrame();
        for (int i = 0; i < 1; i++) {
            uiWindow.Render(renderer,*textRender, 0, 200, glm::vec2(winW, winH));
        }
        // TODO: REVIEW THIS - UIRenderer.Render() may need platform-independent window
        // 🔍 STATIC_CAST LOCATION - Check if UIRenderer needs platform-specific window
        renderer.Render(glm::mat4(1.0f), glm::vec2(winW, winH));

        auto frameEnd = std::chrono::high_resolution_clock::now();
        double frameMs = std::chrono::duration<double, std::milli>(frameEnd - frameStart).count();



        // textRender->ProcessText(1000, 100,1000, "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/arial.ttf",
        //                     "hi; this is a text render test!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", {0,0,0,1}, 90);
        // textRender->ProcessText(1000, 150,1000, "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/arial.ttf",
        //                     "std::snprintf(titleBuf, sizeof(titleBuf), \"FuncciaFrame | %.1f FPS (%.2f ms)\", fps, ms);", {0,0.2,0.5,1}, 40);
        textRender->Render(glm::vec2(winW, winH));

        window->SwapBuffers();

        // Update FPS counter
        framesSinceUpdate++;
        double elapsed = std::chrono::duration<double>(now - fpsWindowStart).count();
        if (elapsed >= 0.5) {
            double fps = framesSinceUpdate / elapsed;
            double ms = 1000.0 / (fps > 0.0 ? fps : 1.0);
            std::snprintf(titleBuf, sizeof(titleBuf), "FuncciaFrame | %.1f FPS (%.2f ms)", fps, ms);
            window->SetTitle(titleBuf);
            fpsWindowStart = now;
            framesSinceUpdate = 0;
        }
    }
    textRender->DebugDrawAtlasToDisk();
    // Clean shutdown
    window->Close();

    return 0;
}