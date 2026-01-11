//
// Created by Wangsicong Wei on 2026-01-09.
//

#ifndef UISTYLE_H
#define UISTYLE_H
#include <string>
#include "SideOffset.h"
#include "UIFluentAPI.h"
#include "../graphic/gl/TextRenderer.h"
namespace Funccia::UI {
    class UIStyle : public UIFluentAPI{
    public:
        std::string tag {};

        // Box model
        SideOffset margin {0,0,0,0};
        SideOffset padding {0,0,0,0};
        SideOffset border_widths {0,0,0,0};
    
        // Sizing & layout
        Axis display_axis {Axis::Vertical};
        Position position {Position::Absolute};
        Sizing sizing {SizingType::Grow, SizingType::Fit, 0, 0};
        Overflow overflow {Overflow::ClipByBorderBox};
    
        // Visibility
        bool is_rendered {true};
        bool invisible_but_occupy_space {false};
    
        // Colors
        vec4 background {0,0,0,0};
        vec4 color {0,0,0,1};          // text color
        vec4 border_color {0,0,0,0};
    
        // Border appearance
        vec4 border_radius {0,0,0,0};
    
        // Shadow
        Shadow shadow {vec4(0,0,0,0), vec2(0,0), 0, 0};
    
        // Text
        //std::string text {};
        int font_size {35};
        Graphic::GL::TextWrap text_wrap_mode {Graphic::GL::TextWrap::Character};
        std::string font_path {"/Users/dvillera/Projects/cpp/FuncciaFrame/graphic/assets/fonts/Libre Baskerville - Regular.ttf"};
        
        
        auto MarginTop(float value) -> UIStyle& override;
        auto MarginLeft(float value) -> UIStyle& override;
        auto MarginRight(float value) -> UIStyle& override;
        auto MarginBottom(float value) -> UIStyle& override;
        auto Margin(float all_values) -> UIStyle& override;
        auto Margin(float topButton, float leftRight) -> UIStyle& override;
        auto Margin(float top, float right, float bottom, float left) -> UIStyle& override;

        auto PaddingTop(float value) -> UIStyle& override;
        auto PaddingLeft(float value) -> UIStyle& override;
        auto PaddingRight(float value) -> UIStyle& override;
        auto PaddingBottom(float value) -> UIStyle& override;
        auto Padding(float all_values) -> UIStyle& override;
        auto Padding(float topButton, float leftRight) -> UIStyle& override;
        auto Padding(float top, float right, float bottom, float left) -> UIStyle& override;

        auto Background(vec4 _color) -> UIStyle& override;
        auto TextColor(vec4 _color) -> UIStyle& override;

        auto BoxShadow(vec2 offSet, float blur, float spread, vec4 _color) -> UIStyle& override;
        auto BoxShadowColor(vec4 _color) -> UIStyle& override;
        auto BoxShadowBlur(float blur) -> UIStyle& override;
        auto BoxShadowSpread(float spread) -> UIStyle& override;
        auto BoxShadowOffset(vec2 offSet) -> UIStyle& override;

        auto BorderRadius(vec4 radius) -> UIStyle& override;
        auto BorderColor(vec4 _color) -> UIStyle& override;
        auto BorderWidth(vec4 width) -> UIStyle& override;

        auto HorizontalFixed(float size) -> UIStyle& override;
        auto HorizontalGrow() -> UIStyle& override;
        auto HorizontalFit() -> UIStyle& override;

        auto VerticalFixed(float size) -> UIStyle& override;
        auto VerticalGrow() -> UIStyle& override;
        auto VerticalFit() -> UIStyle& override;

        auto HorizontalStack() -> UIStyle& override;
        auto VerticalStack() -> UIStyle& override;

        auto SetTag(const std::string& _tag) -> UIStyle& override;

        //auto Text(const std::string& _text) -> UIStyle& override;
        auto FontSize(int size) -> UIStyle& override;
        auto TextWrap(Graphic::GL::TextWrap wrap) -> UIStyle& override;

        auto InvisibleButOccupySpace(bool option) -> UIStyle& override;

    public:
        ~UIStyle() override = default;
    };
}
#endif //