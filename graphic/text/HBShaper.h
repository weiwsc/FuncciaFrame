//
// Created by Wangsicong Wei on 2025-09-28.
//

#ifndef FUNCCIAFRAME_HBSHAPER_H
#define FUNCCIAFRAME_HBSHAPER_H

// FreeType / HarfBuzz

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <hb.h>
#include <hb-ft.h>
#include <string>
#include <vector>

struct HBTextInfo {
    hb_codepoint_t glyph_id;
    hb_position_t x_offset;
    hb_position_t y_offset ;
    hb_position_t x_advance;
    hb_position_t y_advance;
};

class HBShaper {
public:
    HBShaper() = default;
    ~HBShaper() {
        hb_buffer_destroy(buffer);
        hb_font_destroy(m_hbFont);
    };

    void init(FT_Face);
    auto Shape(const std::string &text, std::vector<HBTextInfo>& result)->void;
private:
    hb_font_t *m_hbFont;
    hb_buffer_t *buffer;
};


#endif //FUNCCIAFRAME_HBSHAPER_H