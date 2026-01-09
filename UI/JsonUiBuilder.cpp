//
// Created by Wangsicong Wei on 2026-01-08.
//

#include "JsonUiBuilder.h"

#include "../graphic/gl/TextRenderer.h"

namespace Funccia::UI {
    auto JsonUiBuilder::ParseUI(const std::string &jsonStr, UIElement *root) -> void {
        try {
            auto j = json::parse(jsonStr);
            JsonUiBuilder::ParseElement(j, root);
        }
        catch (const std::exception &e) {
            std::cout << "Error parsing JSON: " << e.what() << std::endl;
        }
    }

    auto JsonUiBuilder::ParseElement(const json &j, UIElement *el) -> void {
        if (j.contains("style")) {
            for (auto &[key, value]: j["style"].items()) {
                if (auto it = JsonUiBuilder::styleHandlers.find(key); it != JsonUiBuilder::styleHandlers.end()) {
                    it->second(el, value);
                }
            }
        }

        if (j.contains("content")) {
            if (auto it = JsonUiBuilder::styleHandlers.find("content"); it != JsonUiBuilder::styleHandlers.end()) {
                it->second(el, j["content"]);
            }
        }

        if (j.contains("children")) {
            for (const auto &child: j["children"]) {
                auto childEl = std::make_unique<UIElement>();
                ParseElement(child, childEl.get());
                el->AddChild(std::move(childEl));
            }
        }
    }

    auto JsonUiBuilder::HandleStyle(nlohmann::basic_json<> styles, UIElement *root) -> void {
        for (auto &[key, value]: styles.items()) {
            if (key == "font-size") {
            }
        }
    }

    auto JsonUiBuilder::ParseColor(const json &v) -> glm::vec4 {
        // Array format: [r, g, b, a] with values 0-1 or 0-255
        if (v.is_array()) {
            if (v.size() >= 3) {
                float r = v[0].get<float>();
                float g = v[1].get<float>();
                float b = v[2].get<float>();
                float a = v.size() >= 4 ? v[3].get<float>() : 1.0f;

                // Assume 0-255 if any value > 1
                if (r > 1 || g > 1 || b > 1 || a > 1) {
                    r /= 255.0f;
                    g /= 255.0f;
                    b /= 255.0f;
                    if (a > 1) a /= 255.0f;
                }
                return {r, g, b, a};
            }
        }

        // String formats: hex or rgba()
        if (v.is_string()) {
            std::string s = v.get<std::string>();

            // Hex format: #RGB, #RGBA, #RRGGBB, #RRGGBBAA
            if (!s.empty() && s[0] == '#') {
                s = s.substr(1);
                unsigned int hex = std::stoul(s, nullptr, 16);

                if (s.length() == 3) {
                    // #RGB
                    float r = ((hex >> 8) & 0xF) / 15.0f;
                    float g = ((hex >> 4) & 0xF) / 15.0f;
                    float b = (hex & 0xF) / 15.0f;
                    return {r, g, b, 1.0f};
                }
                if (s.length() == 4) {
                    // #RGBA
                    float r = ((hex >> 12) & 0xF) / 15.0f;
                    float g = ((hex >> 8) & 0xF) / 15.0f;
                    float b = ((hex >> 4) & 0xF) / 15.0f;
                    float a = (hex & 0xF) / 15.0f;
                    return {r, g, b, a};
                }
                if (s.length() == 6) {
                    // #RRGGBB
                    float r = ((hex >> 16) & 0xFF) / 255.0f;
                    float g = ((hex >> 8) & 0xFF) / 255.0f;
                    float b = (hex & 0xFF) / 255.0f;
                    return {r, g, b, 1.0f};
                }
                if (s.length() == 8) {
                    // #RRGGBBAA
                    float r = ((hex >> 24) & 0xFF) / 255.0f;
                    float g = ((hex >> 16) & 0xFF) / 255.0f;
                    float b = ((hex >> 8) & 0xFF) / 255.0f;
                    float a = (hex & 0xFF) / 255.0f;
                    return {r, g, b, a};
                }
            }

            // rgba(r, g, b, a) or rgb(r, g, b)
            if (s.rfind("rgba(", 0) == 0 || s.rfind("rgb(", 0) == 0) {
                bool hasAlpha = s[3] == 'a';
                size_t start = hasAlpha ? 5 : 4;
                size_t end = s.find(')');
                std::string inner = s.substr(start, end - start);

                // Parse comma-separated values
                float values[4] = {0, 0, 0, 1};
                int i = 0;
                size_t pos = 0;
                while (pos < inner.length() && i < 4) {
                    size_t comma = inner.find(',', pos);
                    if (comma == std::string::npos) comma = inner.length();
                    std::string val = inner.substr(pos, comma - pos);

                    // Trim whitespace
                    size_t vstart = val.find_first_not_of(" ");
                    size_t vend = val.find_last_not_of(" ");
                    if (vstart != std::string::npos) {
                        val = val.substr(vstart, vend - vstart + 1);
                    }

                    values[i++] = std::stof(val);
                    pos = comma + 1;
                }

                // Normalize if 0-255 range
                if (values[0] > 1 || values[1] > 1 || values[2] > 1) {
                    values[0] /= 255.0f;
                    values[1] /= 255.0f;
                    values[2] /= 255.0f;
                }

                return {values[0], values[1], values[2], values[3]};
            }
        }

        return {1, 1, 1, 1}; // default white
    }

    const std::unordered_map<std::string, StyleHandler> JsonUiBuilder::styleHandlers = {
        // Text
        {"content", [](UIElement *el, const json &v) { el->Text(v.get<std::string>()); }},
        {"font-size", [](UIElement *el, const json &v) { el->FontSize(v.get<int>()); }},
        {"text-color", [](UIElement *el, const json &v) { el->TextColor(ParseColor(v)); }},
        {
            "text-wrap", [](UIElement *el, const json &v) {
                // assuming you have a string->enum mapping
                el->TextWrap(v == "wrap" ? Graphic::GL::TextWrap::Character : Graphic::GL::TextWrap::None);
            }
        },

        // Margin
        {
            "margin", [](UIElement *el, const json &v) {
                if (v.is_number()) el->Margin(v.get<float>());
                else if (v.is_array() && v.size() == 2) el->Margin(v[0], v[1]);
                else if (v.is_array() && v.size() == 4) el->Margin(v[0], v[1], v[2], v[3]);
            }
        },
        {"margin-top", [](UIElement *el, const json &v) { el->MarginTop(v.get<float>()); }},
        {"margin-right", [](UIElement *el, const json &v) { el->MarginRight(v.get<float>()); }},
        {"margin-bottom", [](UIElement *el, const json &v) { el->MarginBottom(v.get<float>()); }},
        {"margin-left", [](UIElement *el, const json &v) { el->MarginLeft(v.get<float>()); }},

        // Padding
        {
            "padding", [](UIElement *el, const json &v) {
                if (v.is_number()) el->Padding(v.get<float>());
                else if (v.is_array() && v.size() == 2) el->Padding(v[0], v[1]);
                else if (v.is_array() && v.size() == 4) el->Padding(v[0], v[1], v[2], v[3]);
            }
        },
        {"padding-top", [](UIElement *el, const json &v) { el->PaddingTop(v.get<float>()); }},
        {"padding-right", [](UIElement *el, const json &v) { el->PaddingRight(v.get<float>()); }},
        {"padding-bottom", [](UIElement *el, const json &v) { el->PaddingBottom(v.get<float>()); }},
        {"padding-left", [](UIElement *el, const json &v) { el->PaddingLeft(v.get<float>()); }},

        // Background & Border
        {"background-color", [](UIElement *el, const json &v) { el->Background(ParseColor(v)); }},
        {
            "border-radius", [](UIElement *el, const json &v) {
                if (v.is_number()) {
                    float num = v.get<float>();
                    el->BorderRadius({num, num, num, num});
                } else if (v.is_array() && v.size() == 2) {
                    float topButton = v[0].get<float>();
                    float leftRight = v[1].get<float>();
                    el->BorderRadius({topButton, leftRight, topButton, leftRight});
                } else if (v.is_array() && v.size() == 4) el->BorderRadius({v[0], v[1], v[2], v[3]});
            }
        },
        {"border-color", [](UIElement *el, const json &v) { el->BorderColor(ParseColor(v)); }},
        {
            "border-width", [](UIElement *el, const json &v) {
                if (v.is_number()) {
                    float num = v.get<float>();
                    el->BorderWidth({num, num, num, num});
                } else if (v.is_array() && v.size() == 2) {
                    float topButton = v[0].get<float>();
                    float leftRight = v[1].get<float>();
                    el->BorderWidth({topButton, leftRight, topButton, leftRight});
                } else if (v.is_array() && v.size() == 4) el->BorderWidth({v[0], v[1], v[2], v[3]});
            }
        }, // vec4

        // Box Shadow
        {"box-shadow-color", [](UIElement *el, const json &v) { el->BoxShadowColor(ParseColor(v)); }},
        {"box-shadow-slur", [](UIElement *el, const json &v) { el->BoxShadowBlur(v.get<float>()); }},
        {"box-shadow-spread", [](UIElement *el, const json &v) { el->BoxShadowSpread(v.get<float>()); }},
        {"box-shadow-offset", [](UIElement *el, const json &v) { el->BoxShadowOffset(parseVec2(v)); }},

        // Sizing
        {"width", [](UIElement *el, const json &v) { el->HorizontalFixed(v.get<float>()); }},
        {"height", [](UIElement *el, const json &v) { el->VerticalFixed(v.get<float>()); }},
        {
            "horizontal-sizing", [](UIElement *el, const json &v) {
                auto s = v.get<std::string>();
                if (s == "grow") el->HorizontalGrow();
                else if (s == "fit") el->HorizontalFit();
            }
        },
        {
            "vertical-sizing", [](UIElement *el, const json &v) {
                auto s = v.get<std::string>();
                if (s == "grow") el->VerticalGrow();
                else if (s == "fit") el->VerticalFit();
            }
        },

        // Layout
        {
            "stack", [](UIElement *el, const json &v) {
                auto s = v.get<std::string>();
                if (s == "horizontal") el->HorizontalStack();
                else if (s == "vertical") el->VerticalStack();
            }
        },

        // Misc
        {"invisible", [](UIElement *el, const json &v) { el->InvisibleButOccupySpace(v.get<bool>()); }},
    };
}
