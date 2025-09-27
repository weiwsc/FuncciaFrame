#pragma once
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>
namespace Funccia::Graphic::GL {
    struct FT_LibraryRec_;
    struct FT_FaceRec_;

    class FontAtlas {
    public:
        struct Glyph {
            glm::vec2 uv0;       // top-left (0..1)
            glm::vec2 uv1;       // bottom-right (0..1)
            glm::ivec2 size;     // bitmap size in px
            glm::ivec2 bearing;  // from baseline to left/top
            unsigned int advance; // 1/64 px
        };

        // fontSizePx is the requested pixel height (e.g. 96)
        FontAtlas(const std::string& fontPath, int atlasSize = 1024, int gridSize = 16, int fontSizePx = 96, int pad = 2);
        ~FontAtlas();

        // non-copyable, movable if you need
        FontAtlas(const FontAtlas&) = delete;
        FontAtlas& operator=(const FontAtlas&) = delete;

        unsigned int texture() const { return m_texture; }
        int atlasSize() const { return m_atlasSize; }
        const Glyph* glyph(unsigned char c) const;


        bool saveDebugPNG(const std::string& path,
                      bool showGrid = true,
                      bool showGlyphBoxes = true) const;
    private:
        bool build(const std::string& fontPath);

    private:
        int m_atlasSize;
        int m_gridSize;
        int m_cellSize;
        int m_pad;
        int m_fontSizePx;

        unsigned int m_texture = 0;
        std::map<unsigned char, Glyph> m_glyphs;
        std::vector<unsigned char> m_cpuAtlas;
    };



}