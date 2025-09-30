#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "stb_image.h"
#include "graphic/gl/FontAtlas.h"
#include "graphic/gl/Mesh.h"
#include "graphic/gl/Shader.h"
#include "graphic/gl/ShaderUtil.h"
#include "graphic/gl/TextRenderer.h"
#include "graphic/gl/UIRender.h"
#include "graphic/gl/WindowController.h"
#include "graphic/text/GlyphAtlas.h"
#include "test/TestRun.h"
#include "UI/Window.h"

using namespace std;


// Input processing
void processInput(GLFWwindow *window);

// Mouse picking context
struct MousePickCtx {
    Funccia::Graphic::GL::Mesh* mesh = nullptr;
    float planeZ = 0.0f;
};




int main() {
    // Initialize core systems
    Funccia::Core::AssetController::Instance().Initialize(100 * 1024 * 1024);
    Funccia::Core::ObjectPoolManager::AssetPool = new Funccia::Core::ObjectPool<Funccia::Core::Asset>();

    // Get window and initialize main shader
    GLFWwindow* window = Funccia::Graphic::GL::WindowController::Instance().GetWindow();
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

    // Setup mouse picking
    MousePickCtx ctx;
    ctx.mesh = &mesh;
    ctx.planeZ = 0.0f;
    glfwSetWindowUserPointer(window, &ctx);

    auto cursorPosCB = [](GLFWwindow* w, double xpos, double ypos) {
        if (!glfwGetWindowAttrib(w, GLFW_HOVERED)) return;

        int winW = 1, winH = 1;
        glfwGetWindowSize(w, &winW, &winH);

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

        auto* ctx = static_cast<MousePickCtx*>(glfwGetWindowUserPointer(w));
        if (!ctx || !ctx->mesh) return;

        float denom = rayDir.z;
        if (std::abs(denom) < 1e-6f) return;

        float t = (ctx->planeZ - rayOrigin.z) / denom;
        if (t < 0.0f) return;

        glm::vec3 hit = rayOrigin + t * rayDir;
        ctx->mesh->GetTransform().setPositionXPlane({ hit.x, hit.y });
    };

    glfwSetCursorPosCallback(window, cursorPosCB);


    Funccia::Graphic::GL::FontAtlas atlas("/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/arial.ttf",
                                          32, 1024, 1);
    Funccia::Graphic::GL::TextRenderer text(atlas,
                                            "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/vertex/FontShader1-ver.glsl",
                                            "/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/shaders/fragment/FontShader1-fag.glsl");



    int winW, winH;
    glfwGetFramebufferSize(window, &winW, &winH);

    Funccia::Graphic::GL::GlyphAtlas alt = Funccia::Graphic::GL::GlyphAtlas();
    alt.CreateAtlas("/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/arial.ttf", 1200, 1200);
    alt.WriteAtlasToDisk();


    renderer.Render(glm::mat4(1.0f), glm::vec2(winW, winH));

    //atlas.saveDebugPNG("font_atlas_debug.png", /*showGrid=*/true, /*showGlyphBoxes=*/true);

    // Timing variables
    double lastTime = glfwGetTime();
    int framesSinceUpdate = 0;
    double fpsWindowStart = lastTime;
    char titleBuf[128];

#ifdef FF_UI_LAZY_LAYOUT
    uiWindow.InitLayout(0, 0, glm::vec2(winW, winH));
#endif
    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        float dt = static_cast<float>(now - lastTime);
        lastTime = now;

        // Process input
        const float rotSpeed = 120.0f;
        const float moveSpeed = 3.0f;
        glfwPollEvents();
        processInput(window);

        // Mesh controls
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) mesh.Rotate(-rotSpeed * dt, 0, 0);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) mesh.Rotate(rotSpeed * dt, 0, 0);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) mesh.Rotate(0, -rotSpeed * dt, 0);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) mesh.Rotate(0, rotSpeed * dt, 0);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) mesh.Rotate(0, 0, -rotSpeed * dt);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) mesh.Rotate(0, 0, rotSpeed * dt);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) mesh.MoveUp(moveSpeed * dt);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) mesh.MoveUp(-moveSpeed * dt);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) mesh.MoveRight(-moveSpeed * dt);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) mesh.MoveRight(moveSpeed * dt);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) mesh.MoveForward(moveSpeed * dt);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) mesh.MoveForward(-moveSpeed * dt);

        // Clear and render 3D scene
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mesh.Draw(&shader);



        glfwGetFramebufferSize(window, &winW, &winH);

        auto frameStart = std::chrono::high_resolution_clock::now();

        renderer.BeginFrame();

        //text.clear();
        for (int i = 0; i < 1; i++) {
            uiWindow.RenderProfile(renderer, text, i * 10, i * 10, glm::vec2(winW, winH));
        }

        // Only time the GPU work for the actual rendering
        GLuint query;
        glGenQueries(1, &query);
        glBeginQuery(GL_TIME_ELAPSED, query);

        auto start = std::chrono::high_resolution_clock::now();
        renderer.Render(glm::mat4(1.0f), glm::vec2(winW, winH));
        auto end = std::chrono::high_resolution_clock::now();
        double RendererRenderCPU = std::chrono::duration<double, std::milli>(end - start).count();

        glEndQuery(GL_TIME_ELAPSED);

        GLuint64 elapsedGPU;
        glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsedGPU);
        double gpuMs = elapsedGPU / 1000000.0;

        auto frameEnd = std::chrono::high_resolution_clock::now();
        double frameMs = std::chrono::duration<double, std::milli>(frameEnd - frameStart).count();

        printf("Frame: %.3f ms | GPU draw: %.3f ms | CPU submit: %.3f ms\n",
               frameMs, gpuMs, RendererRenderCPU);

        // text.queue("This is sample text", 225, 25, 1.0f, {0,0,0});
        // text.queue("(C) LearnOpenGL.com Lorem Ipsum is simply dummy text of the printing and typesetting industry. "
        //            "Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took "
        //            "a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but "
        //            "also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with "
        //            "the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software "
        //            "like Aldus PageMaker including versions of Lorem Ipsum.", 225, 570, 0.35f, {0,0,0}, 600);
        // //text.queue("This is text rendering test", 25, 540, 0.2f, {0,0,0});
        // text.queue("The quick brown fox jumps", 225, 120, 0.5f, {0.2f,0.4f,0.8f});
        text.flush(window);


        glfwSwapBuffers(window);

        // Update FPS counter
        framesSinceUpdate++;
        double elapsed = now - fpsWindowStart;
        if (elapsed >= 0.5) {
            double fps = framesSinceUpdate / elapsed;
            double ms = 1000.0 / (fps > 0.0 ? fps : 1.0);
            std::snprintf(titleBuf, sizeof(titleBuf), "FuncciaFrame | %.1f FPS (%.2f ms)", fps, ms);
            glfwSetWindowTitle(window, titleBuf);
            fpsWindowStart = now;
            framesSinceUpdate = 0;
        }

    }


    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

