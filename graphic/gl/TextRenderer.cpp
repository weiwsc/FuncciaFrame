
#include "TextRenderer.h"

#include <memory>
#include <unordered_set>
#include <freetype/freetype.h>
#include "../text/HBShaper.h"

namespace Funccia::Graphic::GL {
    void TextRenderer::ProcessText(const std::string &fontPath, const std::string& text) {
        std::vector<AtlasCell> shapedText {};
        ShapeText(fontPath, text, shapedText);
    }

    void TextRenderer::ShapeText(const std::string &fontPath, const std::string &text, std::vector<AtlasCell>& shapedText) {
        FT_Face face = m_atlas->LoadFont(fontPath);
        std::unique_ptr<HBShaper> shaper = std::make_unique<HBShaper>();
        shaper->init(face);
        std::vector<HBTextInfo> results;
        shaper->Shape(text, results);

        std::unordered_set<hb_codepoint_t> glyphToAdd {};
        for (auto result: results) {
            if(!m_atlas->GlyphExists(result.glyph_id)) {
                glyphToAdd.insert(result.glyph_id);
            }
        }
        if (!glyphToAdd.empty()) {
            m_atlas->CopyBitmapToAtlas(face, glyphToAdd);
        }
        for (auto result: results) {
            shapedText.push_back(m_atlas->GetGlyph(result.glyph_id));
        }
    }

    void TextRenderer::PositionText(const std::vector<AtlasCell> &shapedText, float width, float height) {

    }
}
