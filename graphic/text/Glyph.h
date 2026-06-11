//
// Created by Wangsicong Wei on 2025-09-28.
//

#ifndef FUNCCIAFRAME_GLYPH_H
#define FUNCCIAFRAME_GLYPH_H
#include <glm/vec2.hpp>
#include <glm/detail/type_vec2.hpp>

// FreeType / HarfBuzz
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <hb.h>
#include <hb-ft.h>

namespace Funccia::Graphic {
    using vec2 = glm::vec2;

    struct Glyph {
        vec2 uv0{}; //bottom left
        vec2 uv1{}; //upper right
        vec2 size{};
        vec2 bearing{};
    };
}
#endif //FUNCCIAFRAME_GLYPH_H
