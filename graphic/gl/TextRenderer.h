#pragma once
#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Shader.h"
#include <glfw/glfw3.h>
#include "FontAtlas.h"
struct GLFWwindow; // forward-declare to avoid pulling GLFW into the header

namespace Funccia::Graphic::GL {

    class FontAtlas;          // from your FT+HB atlas
    class Shader;             // your Shader (from Shader.h)

    struct CharVertex {
        glm::vec2 pos;   // pixel-space (top-left origin)
        glm::vec2 uv;
        glm::vec3 color; // linear or sRGB (match your pipeline)
    };

    class TextRenderer {
    public:
        // NOTE: FontAtlas is non-const because we lazily pack/upload glyphs
        TextRenderer(FontAtlas& atlas, const char* vertPath, const char* fragPath);
        ~TextRenderer();

        // Simple (no wrapping)
        void queue(const std::string& text, float x, float y, float scale, const glm::vec3& color);

        // Greedy word-wrapping on spaces; returns text block height in pixels
        auto queue(const std::string& text,
                   float x, float y,
                   float scale,
                   const glm::vec3& color,
                   float width_limit) -> float;

        void clear();
        void flush(GLFWwindow* window); // sets viewport to framebuffer size, draws, clears

        void setHalfTexelInset(float halfTexelUV) { m_halfTexelUV = halfTexelUV; }

        void debugQueue(const std::string& text, float x, float y, float scale, const glm::vec3& color) {
    printf("\n=== DEBUG QUEUE: '%s' at (%.1f,%.1f) scale=%.2f ===\n",
           text.c_str(), x, y, scale);

    size_t vertsBefore = m_vertices.size();
    queue(text, x, y, scale, color);
    size_t vertsAfter = m_vertices.size();

    printf("Generated %zu vertices (%zu triangles)\n",
           vertsAfter - vertsBefore, (vertsAfter - vertsBefore) / 3);

    // Print first few vertices
    for (size_t i = vertsBefore; i < std::min(vertsBefore + 6, vertsAfter); ++i) {
        const auto& v = m_vertices[i];
        printf("  v[%zu]: pos=(%.1f,%.1f), uv=(%.3f,%.3f), color=(%.2f,%.2f,%.2f)\n",
               i, v.pos.x, v.pos.y, v.uv.x, v.uv.y, v.color.r, v.color.g, v.color.b);
    }
}

// 2. Debug OpenGL state during flush
void debugFlush(GLFWwindow* window) {
    printf("\n=== DEBUG FLUSH ===\n");
    printf("Vertices to draw: %zu\n", m_vertices.size());

    if (m_vertices.empty()) {
        printf("No vertices to draw!\n");
        return;
    }

    // Check GL errors before we start
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("GL error before flush: 0x%x\n", err);
    }

    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    printf("Framebuffer size: %dx%d\n", fbw, fbh);

    // Check if atlas texture is valid
    glBindTexture(GL_TEXTURE_2D, m_atlas.texture());
    int texWidth = 0, texHeight = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
    printf("Atlas texture: %dx%d (ID=%u)\n", texWidth, texHeight, m_atlas.texture());

    // Check shader
    printf("Shader ID: %u\n", m_shader.getID());
    printf("Uniform locations: viewSize=%d, sampler=%d\n", m_locViewSize, m_locSampler);

    // Call original flush
    flush(window);

    // Check for GL errors after
    err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("GL error after flush: 0x%x\n", err);
    }
}

// 3. Simple test with known glyph
void testSingleGlyph(GLFWwindow* window) {
    clear();

    // Test a single 'A' character
    printf("\n=== TESTING SINGLE GLYPH 'A' ===\n");

    uint32_t glyphIdx = m_atlas.charToGlyphIndex('A');
    printf("'A' maps to glyph index: %u\n", glyphIdx);

    const auto* glyph = m_atlas.ensureGlyph(glyphIdx);
    if (!glyph) {
        printf("ERROR: Could not ensure glyph for 'A'\n");
        return;
    }

    printf("Glyph 'A': size=(%d,%d), bearing=(%d,%d)\n",
           glyph->size.x, glyph->size.y, glyph->bearing.x, glyph->bearing.y);
    printf("UV: (%.3f,%.3f) to (%.3f,%.3f)\n",
           glyph->uv0.x, glyph->uv0.y, glyph->uv1.x, glyph->uv1.y);

    // Manually create vertices for this glyph
    float x = 100.0f, y = 100.0f, scale = 1.0f;
    float w = glyph->size.x * scale;
    float h = glyph->size.y * scale;
    float xpos = x + glyph->bearing.x * scale;
    float ypos = y - glyph->bearing.y * scale + m_atlas.ascent() * scale;

    glm::vec3 color(1.0f, 1.0f, 1.0f);
    emitGlyph(xpos, ypos, w, h, glyph->uv0.x, glyph->uv0.y, glyph->uv1.x, glyph->uv1.y, color);

    printf("Emitted quad: pos=(%.1f,%.1f), size=(%.1f,%.1f)\n", xpos, ypos, w, h);

    debugFlush(window);
}

// 4. Test with simple shader that shows raw texture
void testWithDebugShader() {
    // You can create a simple debug shader that just shows texture content:
    // Fragment shader:
    // #version 330 core
    // in vec2 vUV;
    // out vec4 FragColor;
    // uniform sampler2D fontAtlas;
    // void main() {
    //     float val = texture(fontAtlas, vUV).r; // or .a depending on swizzle
    //     FragColor = vec4(val, val, val, 1.0);
    // }

    printf("Create a debug shader to test raw texture sampling\n");
}
        // Add this debug method to TextRenderer class

void debugRenderPipeline(GLFWwindow* window, const std::string& text) {
    printf("\n=== DEBUGGING RENDER PIPELINE ===\n");

    clear();

    // 1. Test basic queue operation
    printf("1. Testing queue with: '%s'\n", text.c_str());
    size_t vertsBefore = m_vertices.size();
    queue(text, 100.0f, 100.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    size_t vertsAfter = m_vertices.size();

    printf("Generated %zu vertices\n", vertsAfter - vertsBefore);

    if (vertsAfter == vertsBefore) {
        printf("ERROR: No vertices generated!\n");
        return;
    }

    // 2. Check first few vertices
    printf("First 3 vertices:\n");
    for (size_t i = 0; i < std::min(size_t(3), m_vertices.size()); ++i) {
        const auto& v = m_vertices[i];
        printf("  [%zu] pos=(%.1f,%.1f) uv=(%.4f,%.4f) color=(%.2f,%.2f,%.2f)\n",
               i, v.pos.x, v.pos.y, v.uv.x, v.uv.y, v.color.r, v.color.g, v.color.b);
    }

    // 3. Check framebuffer and viewport
    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    printf("Framebuffer: %dx%d\n", fbw, fbh);

    // 4. Check atlas texture
    printf("Atlas texture ID: %u, size: %d\n", m_atlas.texture(), m_atlas.atlasSize());

    // 5. Test GL state before flush
    glBindTexture(GL_TEXTURE_2D, m_atlas.texture());
    int texW = 0, texH = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texW);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texH);
    printf("GL Texture dimensions: %dx%d\n", texW, texH);

    // 6. Check shader program
    printf("Shader program ID: %u\n", m_shader.getID());

    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    printf("Current GL program before use: %d\n", currentProgram);

    m_shader.use();
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    printf("Current GL program after use: %d\n", currentProgram);

    // 7. Check uniform locations
    printf("Uniform locations: viewSize=%d, sampler=%d\n", m_locViewSize, m_locSampler);

    if (m_locViewSize == -1) {
        printf("WARNING: u_viewSize uniform not found in shader!\n");
    }
    if (m_locSampler == -1) {
        printf("WARNING: fontAtlas uniform not found in shader!\n");
    }

    // 8. Now call flush and check for errors
    printf("Calling flush...\n");

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) printf("GL error before flush: 0x%x\n", err);

    flush(window);

    err = glGetError();
    if (err != GL_NO_ERROR) printf("GL error after flush: 0x%x\n", err);

    printf("=== END DEBUG ===\n\n");
}

// Also add this simple test that bypasses shaping
void debugManualQuad(GLFWwindow* window) {
    printf("\n=== TESTING MANUAL QUAD ===\n");

    clear();

    // Manually create a simple quad to test the entire pipeline
    // Use known good UV coordinates from your debug output
    float x = 200.0f, y = 200.0f, w = 50.0f, h = 50.0f;
    float u0 = 0.037f, v0 = 0.002f, u1 = 0.070f, v1 = 0.036f; // 'A' UVs from your debug
    glm::vec3 color(1.0f, 0.0f, 0.0f); // Red for visibility

    emitGlyph(x, y, w, h, u0, v0, u1, v1, color);

    printf("Manual quad: pos=(%.1f,%.1f), size=(%.1f,%.1f)\n", x, y, w, h);
    printf("UV: (%.3f,%.3f) to (%.3f,%.3f)\n", u0, v0, u1, v1);
    printf("Generated %zu vertices\n", m_vertices.size());

    if (!m_vertices.empty()) {
        printf("First vertex: pos=(%.1f,%.1f), uv=(%.3f,%.3f)\n",
               m_vertices[0].pos.x, m_vertices[0].pos.y,
               m_vertices[0].uv.x, m_vertices[0].uv.y);
    }

    flush(window);
}

    private:
        FontAtlas& m_atlas;
        Shader     m_shader;

        unsigned int m_vao = 0;
        unsigned int m_vbo = 0;

        // Uniform locations
        int m_locViewSize = -1;
        int m_locSampler  = -1;

        float m_halfTexelUV = 0.0f; // 0.5 / atlasSize

        std::vector<CharVertex> m_vertices;

        // Emit a single glyph quad (top-left origin)
        void emitGlyph(float xpos, float yposTop, float w, float h,
                       float u0, float v0, float u1, float v1,
                       const glm::vec3& color);



    };


} // namespace Funccia::Graphic::GL