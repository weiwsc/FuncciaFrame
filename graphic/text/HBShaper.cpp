//
// Created by Wangsicong Wei on 2025-09-28.
//

#include "HBShaper.h"

void HBShaper::init(FT_Face ft_face) {
    m_hbFont = hb_ft_font_create_referenced(ft_face);

    buffer = hb_buffer_create();

    hb_buffer_allocation_successful(buffer);
}

auto HBShaper::Shape(const std::string &text, std::vector<HBTextInfo> &result) -> void {
    hb_buffer_reset(buffer);
    buffer = hb_buffer_create();

    hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
    hb_buffer_set_script(buffer, HB_SCRIPT_LATIN);
    hb_buffer_set_language(buffer, hb_language_from_string("en", -1));
    size_t len = text.size();
    hb_buffer_add_utf8(buffer, text.c_str(), len, 0, len);

    hb_shape(m_hbFont, buffer, nullptr, 0);

    unsigned int glyph_count;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buffer, &glyph_count);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buffer, &glyph_count);


    for (unsigned int i = 0; i < glyph_count; i++) {
        result.push_back({
            glyph_info[i].codepoint,
            glyph_pos[i].x_offset,
            glyph_pos[i].y_offset,
            glyph_pos[i].x_advance,
            glyph_pos[i].y_advance
        });
    }
}
