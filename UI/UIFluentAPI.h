//
// Created by Wangsicong Wei on 2026-01-09.
//

#ifndef UI_UIFLUENTAPI_H
#define UI_UIFLUENTAPI_H
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <string>

#include "../graphic/RenderTypes.h"

namespace vva::UI {
    enum class Tag;
    using vec2 = glm::vec2;
    using vec4 = glm::vec4;
    class UIFluentAPI {
    public:
        virtual ~UIFluentAPI() = default;
        auto virtual MarginTop(float value) -> UIFluentAPI& = 0;
        auto virtual MarginLeft(float value) -> UIFluentAPI& = 0;
        auto virtual MarginRight(float value) -> UIFluentAPI& = 0;
        auto virtual MarginBottom(float value) -> UIFluentAPI& = 0;
        auto virtual Margin(float all_values) -> UIFluentAPI& = 0;
        auto virtual Margin(float topButton, float leftRight) -> UIFluentAPI& = 0;
        auto virtual Margin(float top, float right, float bottom, float left) -> UIFluentAPI& = 0;

        auto virtual PaddingTop(float value) -> UIFluentAPI& = 0;
        auto virtual PaddingLeft(float value) -> UIFluentAPI& = 0;
        auto virtual PaddingRight(float value) -> UIFluentAPI& = 0;
        auto virtual PaddingBottom(float value) -> UIFluentAPI& = 0;
        auto virtual Padding(float all_values) -> UIFluentAPI& = 0;
        auto virtual Padding(float topButton, float leftRight) -> UIFluentAPI& = 0;
        auto virtual Padding(float top, float right, float bottom, float left) -> UIFluentAPI& = 0;

        auto virtual Background(vec4 _color) -> UIFluentAPI& = 0;
        auto virtual TextColor(vec4 _color) -> UIFluentAPI& = 0;

        auto virtual BoxShadow(vec2 offSet, float blur, float spread, vec4 _color) -> UIFluentAPI& = 0;
        auto virtual BoxShadowColor(vec4 _color) -> UIFluentAPI& = 0;
        auto virtual BoxShadowBlur(float blur) -> UIFluentAPI& = 0;
        auto virtual BoxShadowSpread(float spread) -> UIFluentAPI& = 0;
        auto virtual BoxShadowOffset(vec2 offSet) -> UIFluentAPI& = 0;

        auto virtual BorderRadius(vec4 radius) -> UIFluentAPI& = 0;
        auto virtual BorderColor(vec4 _color) -> UIFluentAPI& = 0;
        auto virtual BorderWidth(vec4 width) -> UIFluentAPI& = 0;

        auto virtual HorizontalFixed(float size) -> UIFluentAPI& = 0;
        auto virtual HorizontalGrow() -> UIFluentAPI& = 0;
        auto virtual HorizontalFit() -> UIFluentAPI& = 0;

        auto virtual VerticalFixed(float size) -> UIFluentAPI& = 0;
        auto virtual VerticalGrow() -> UIFluentAPI& = 0;
        auto virtual VerticalFit() -> UIFluentAPI& = 0;

        auto virtual HorizontalStack() -> UIFluentAPI& = 0;
        auto virtual VerticalStack() -> UIFluentAPI& = 0;

        auto virtual SetTag(const std::string& _tag) -> UIFluentAPI& = 0;

        //auto virtual Text(const std::string& _text) -> UIFluentAPI& = 0;
        auto virtual FontSize(int size) -> UIFluentAPI& = 0;
        auto virtual TextWrap(gfx::TextWrap wrap) -> UIFluentAPI& = 0;

        auto virtual InvisibleButOccupySpace(bool option) -> UIFluentAPI& = 0;

        auto virtual HandleScroll(bool option) -> UIFluentAPI& = 0;
    };
}

#endif //
