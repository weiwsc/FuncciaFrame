//
// Created by Codex on 2026-06-10.
//

#ifndef FUNCCIAFRAME_RENDERTYPES_H
#define FUNCCIAFRAME_RENDERTYPES_H

#include <string>

#include "../engine/gfx/MathTypes.h"

namespace vva::gfx {
    enum class TextWrap {
        None,
        Word,
        Character
    };

    struct FrameContext {
        glm::ivec2 framebufferSize {0, 0};
        vec4 clearColor {1.0f, 1.0f, 1.0f, 1.0f};
    };

    struct UiDrawRect {
        vec4 borderBox {0, 0, 0, 0};
        vec4 backgroundColor {0, 0, 0, 0};
        vec4 borderRadius {0, 0, 0, 0};
        vec4 shadowProperties {0, 0, 0, 0};
        vec4 shadowColor {0, 0, 0, 0};
        vec4 borderWidths {0, 0, 0, 0};
        vec4 borderColor {0, 0, 0, 0};
        vec4 clippingBox {0, 0, 0, 0};
    };

    struct TextDrawRequest {
        float x {0};
        float y {0};
        float width {0};
        TextWrap textWrap {TextWrap::Character};
        vec4 clippingBox {0, 0, 0, 0};
        std::string text {};
        vec4 color {0, 0, 0, 1};
        float pixelSize {16};
        std::string fontPath {};
    };
}

#endif //FUNCCIAFRAME_RENDERTYPES_H
