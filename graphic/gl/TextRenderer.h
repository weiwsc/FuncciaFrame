#pragma once
#include "../text/GlyphAtlas.h"

struct GLFWwindow; // forward-declare to avoid pulling GLFW into the header

namespace Funccia::Graphic::GL {
    class TextRenderer {
        TextRenderer();

        void ProcessText(const std::string &fontPath, const std::string& text);
        void ShapeText(const std::string &fontPath, const std::string& text, std::vector<AtlasCell>& shapedText);
        void PositionText(const std::vector<AtlasCell>& shapedText, float width, float height);
    private:
        std::unique_ptr<GlyphAtlas> m_atlas;
    };

} // namespace Funccia::Graphic::GL