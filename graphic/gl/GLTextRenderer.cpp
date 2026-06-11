
#include "GLTextRenderer.h"

#include <memory>
#include <unordered_set>
#include <freetype/freetype.h>
#include "../text/HBShaper.h"

namespace Funccia::Graphic::GL {
    GLTextRenderer::GLTextRenderer() {
        m_atlas = std::make_unique<Funccia::Graphic::GL::GLGlyphAtlas>();
        m_atlas->CreateAtlas(2048, 2048, 4);
    }

    GLTextRenderer::~GLTextRenderer() {
        Cleanup();
    }

    float GLTextRenderer::ProcessText(float x, float y, float w, TextWrap text_wrap, vec4 clippingBox,
                                      const std::string &text, vec4 color, float pixel,
                                      const std::string &fontPath, bool dry_run) {
        const std::vector<ShapedGlyph>& shapedText = ShapeText(fontPath, text);

        float scale = pixel / 96.0f; //96 is the size used to init the font, change this later
        vec2 cursor {x, y + pixel};

        for (const auto& [uv, size, bearing, advance, offset, layer]: shapedText) {
             if ( w>=0 && cursor.x + advance.x * scale - x > w) {
                if (text_wrap == TextWrap::Character) {
                     cursor.x = x;
                     cursor.y += pixel;
                 }
                 else if (text_wrap == TextWrap::None) {
                     //ProcessText(cursor.x, cursor.y - pixel, -1, text_wrap, "...", color, pixel, fontPath);
                     break;
                 }
             }
            float origin_x = cursor.x + offset.x * scale;
            float origin_y = cursor.y + offset.y * scale;

            if (!dry_run) {
                Collect({origin_x, origin_y,
                    size.x * scale, size.y * scale,      // Scale size
                    bearing.x * scale, bearing.y * scale, // Scale bearing
                    uv.x, uv.y, uv.z, uv.w,
                    color.r, color.g, color.b, color.a,
                    static_cast<float>(layer),
                    clippingBox.x, clippingBox.y, clippingBox.z, clippingBox.w
                });
            }

            cursor.x += advance.x * scale;  // Scale advance
            cursor.y += advance.y * scale;
        }
        cursor.y += pixel/4; //makes the text looks more vertically centered in the text box
        return cursor.y;
    }

    auto GLTextRenderer::ShapeText(const std::string &fontPath, const std::string &text)->const std::vector<ShapedGlyph>& {
        if (auto it = shape_cache.find({fontPath, text}); it != shape_cache.end()) {
            return it->second;
        }

        HBShaper *shaper;
        if (auto it = shapers.find(fontPath); it != shapers.end()) {
            shaper = it->second.get();
        }
        else {
            FT_Face face = m_atlas->LoadFont(fontPath);
            shapers[fontPath] = std::make_unique<HBShaper>();
            shaper = shapers[fontPath].get();
            shaper->init(face);
        }
        std::vector<HBTextInfo> results;
        shaper->Shape(text, results);

        std::unordered_set<hb_codepoint_t> glyphToAdd{};
        for (auto result: results) {
            if (!m_atlas->GlyphExists(result.glyph_id)) {
                glyphToAdd.insert(result.glyph_id);
            }
        }
        if (!glyphToAdd.empty()) {
            FT_Face face = m_atlas->LoadFont(fontPath);
            m_atlas->CopyBitmapToAtlas(face, glyphToAdd);
        }
        std::vector<ShapedGlyph> shapedText;
        for (auto result: results) {
            auto glyph = m_atlas->GetGlyph(result.glyph_id);
            float conversionFactor = 64.0f;
            shapedText.push_back({
                .uv = glyph.uv,
                .size = glyph.size,
                .bearing = glyph.bearing,
                .advance = {result.x_advance / conversionFactor, result.y_advance / conversionFactor},
                .offset = {result.x_offset / conversionFactor, result.y_offset / conversionFactor},
                .layer = glyph.layer,
            });
        }
        auto [it, _] = shape_cache.emplace(std::make_pair(fontPath, text), std::move(shapedText));
        return it->second;
    }

    void GLTextRenderer::PositionText(const std::vector<AtlasCell> &shapedText, float width, float height) {
    }

    auto GLTextRenderer::Initialize(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) -> bool {
        {
            // Initialize shader
            m_shader.initialize(vertexShaderPath, fragmentShaderPath);

            CreateGeometry();
            return true;
        }
    }

    void GLTextRenderer::CreateGeometry() {
        // Quad vertices (0,0) to (1,1)
        float quadVertices[] = {
            0.0f, 0.0f, // bottom-left
            1.0f, 0.0f, // bottom-right
            1.0f, 1.0f, // top-right

            0.0f, 0.0f, // bottom-left
            1.0f, 1.0f, // top-right
            0.0f, 1.0f // top-left
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_instanceVBO);

        glBindVertexArray(m_VAO);


        m_shader.use();

        GLint loc = glGetUniformLocation(m_shader.getID(), "uAtlas");
        glUniform1i(loc, 0);

        // Quad vertices
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        // Instance data buffer setup for one glyph quad.
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);

        const size_t instanceSize = 19 * sizeof(float);
        size_t offset = 0;

        // origin (location 1)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 1);
        offset += 2 * sizeof(float);

        // size (location 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);
        offset += 2 * sizeof(float);

        // bearing (location 3)
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);
        offset += 2 * sizeof(float);

        // uv rect (location 4)
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(4);
        glVertexAttribDivisor(4, 1);
        offset += 4 * sizeof(float);

        // color (location 5)
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(5);
        glVertexAttribDivisor(5, 1);
        offset += 4 * sizeof(float);

        // layer (location 6)
        glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(6);
        glVertexAttribDivisor(6, 1);
        offset += sizeof(float);

        // clipping box (location 7)
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, instanceSize, (void *) offset);
        glEnableVertexAttribArray(7);
        glVertexAttribDivisor(7, 1);

    }

    auto GLTextRenderer::MeasureText(const TextDrawRequest& request) -> float {
        return ProcessText(request.x, request.y, request.width, request.textWrap, request.clippingBox,
                           request.text, request.color, request.pixelSize, request.fontPath, true);
    }

    auto GLTextRenderer::SubmitText(const TextDrawRequest& request) -> void {
        ProcessText(request.x, request.y, request.width, request.textWrap, request.clippingBox,
                    request.text, request.color, request.pixelSize, request.fontPath, false);
    }

    auto GLTextRenderer::Render(const glm::vec2 &screenSize) -> void {
        if (instanceData.empty()) return;

        const size_t floatsPerInstance = 19;
        const size_t instanceCount = instanceData.size() / floatsPerInstance;

        if (instanceData.size() % floatsPerInstance != 0) {
            std::cerr << "Warning: Instance data size not divisible by " << floatsPerInstance << std::endl;
            return;
        }

        m_shader.use();
        glUniform2f(glGetUniformLocation(m_shader.getID(), "u_viewSize"),
                    screenSize.x, screenSize.y);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_atlas->GetAtlasTextureArray());
        glUniform1i(glGetUniformLocation(m_shader.getID(), "uAtlas"), 0);
        glUniform1f(glGetUniformLocation(m_shader.getID(), "uSDFPxRange"), 8.0f);
        glUniform2f(glGetUniformLocation(m_shader.getID(), "u_viewSize"),
                    screenSize.x, screenSize.y);

        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);

        // Orphan the buffer to avoid stalls
        size_t bufferSize = instanceData.size() * sizeof(float);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STREAM_DRAW); // Orphan
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, instanceData.data()); // Upload

        //without orphaning
        //    glBufferData(GL_ARRAY_BUFFER,
        //            instanceData.size() * sizeof(float),
        //            instanceData.data(),
        //          GL_DYNAMIC_DRAW);

        glBindVertexArray(m_VAO);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instanceCount);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(0);
    }

    void GLTextRenderer::DebugDrawAtlasToDisk() {
        m_atlas->WriteAtlasToDisk(0);
    }
}
