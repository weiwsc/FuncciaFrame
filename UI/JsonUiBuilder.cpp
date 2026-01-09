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
                    it->second(&(el->Style()), value);
                }
            }
        }

        if (j.contains("content")) {
            if (auto it = JsonUiBuilder::styleHandlers.find("content"); it != JsonUiBuilder::styleHandlers.end()) {
                it->second(&(el->Style()), j["content"]);
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
        {"content", [](UIFluentAPI *elf, const json &v) { elf->Text(v.get<std::string>()); }},
        {"font-size", [](UIFluentAPI *elf, const json &v) { elf->FontSize(v.get<int>()); }},
        {"text-color", [](UIFluentAPI *elf, const json &v) { elf->TextColor(ParseColor(v)); }},
        {
            "text-wrap", [](UIFluentAPI *elf, const json &v) {
                // assuming you have a string->enum mapping
                elf->TextWrap(v == "wrap" ? Graphic::GL::TextWrap::Character : Graphic::GL::TextWrap::None);
            }
        },

        // Margin
        {
            "margin", [](UIFluentAPI *elf, const json &v) {
                if (v.is_number()) elf->Margin(v.get<float>());
                else if (v.is_array() && v.size() == 2) elf->Margin(v[0], v[1]);
                else if (v.is_array() && v.size() == 4) elf->Margin(v[0], v[1], v[2], v[3]);
            }
        },
        {"margin-top", [](UIFluentAPI *elf, const json &v) { elf->MarginTop(v.get<float>()); }},
        {"margin-right", [](UIFluentAPI *elf, const json &v) { elf->MarginRight(v.get<float>()); }},
        {"margin-bottom", [](UIFluentAPI *elf, const json &v) { elf->MarginBottom(v.get<float>()); }},
        {"margin-left", [](UIFluentAPI *elf, const json &v) { elf->MarginLeft(v.get<float>()); }},

        // Padding
        {
            "padding", [](UIFluentAPI *elf, const json &v) {
                if (v.is_number()) elf->Padding(v.get<float>());
                else if (v.is_array() && v.size() == 2) elf->Padding(v[0], v[1]);
                else if (v.is_array() && v.size() == 4) elf->Padding(v[0], v[1], v[2], v[3]);
            }
        },
        {"padding-top", [](UIFluentAPI *elf, const json &v) { elf->PaddingTop(v.get<float>()); }},
        {"padding-right", [](UIFluentAPI *elf, const json &v) { elf->PaddingRight(v.get<float>()); }},
        {"padding-bottom", [](UIFluentAPI *elf, const json &v) { elf->PaddingBottom(v.get<float>()); }},
        {"padding-left", [](UIFluentAPI *elf, const json &v) { elf->PaddingLeft(v.get<float>()); }},

        // Background & Border
        {"background-color", [](UIFluentAPI *elf, const json &v) { elf->Background(ParseColor(v)); }},
        {
            "border-radius", [](UIFluentAPI *elf, const json &v) {
                if (v.is_number()) {
                    float num = v.get<float>();
                    elf->BorderRadius({num, num, num, num});
                } else if (v.is_array() && v.size() == 2) {
                    float topButton = v[0].get<float>();
                    float leftRight = v[1].get<float>();
                    elf->BorderRadius({topButton, leftRight, topButton, leftRight});
                } else if (v.is_array() && v.size() == 4) elf->BorderRadius({v[0], v[1], v[2], v[3]});
            }
        },
        {"border-color", [](UIFluentAPI *elf, const json &v) { elf->BorderColor(ParseColor(v)); }},
        {
            "border-width", [](UIFluentAPI *elf, const json &v) {
                if (v.is_number()) {
                    float num = v.get<float>();
                    elf->BorderWidth({num, num, num, num});
                } else if (v.is_array() && v.size() == 2) {
                    float topButton = v[0].get<float>();
                    float leftRight = v[1].get<float>();
                    elf->BorderWidth({topButton, leftRight, topButton, leftRight});
                } else if (v.is_array() && v.size() == 4) elf->BorderWidth({v[0], v[1], v[2], v[3]});
            }
        }, // vec4

        // Box Shadow
        {"box-shadow-color", [](UIFluentAPI *elf, const json &v) { elf->BoxShadowColor(ParseColor(v)); }},
        {"box-shadow-slur", [](UIFluentAPI *elf, const json &v) { elf->BoxShadowBlur(v.get<float>()); }},
        {"box-shadow-spread", [](UIFluentAPI *elf, const json &v) { elf->BoxShadowSpread(v.get<float>()); }},
        {"box-shadow-offset", [](UIFluentAPI *elf, const json &v) { elf->BoxShadowOffset(parseVec2(v)); }},

        // Sizing
        {"width", [](UIFluentAPI *elf, const json &v) { elf->HorizontalFixed(v.get<float>()); }},
        {"height", [](UIFluentAPI *elf, const json &v) { elf->VerticalFixed(v.get<float>()); }},
        {
            "horizontal-sizing", [](UIFluentAPI *elf, const json &v) {
                auto s = v.get<std::string>();
                if (s == "grow") elf->HorizontalGrow();
                else if (s == "fit") elf->HorizontalFit();
            }
        },
        {
            "vertical-sizing", [](UIFluentAPI *elf, const json &v) {
                auto s = v.get<std::string>();
                if (s == "grow") elf->VerticalGrow();
                else if (s == "fit") elf->VerticalFit();
            }
        },

        // Layout
        {
            "stack", [](UIFluentAPI *elf, const json &v) {
                auto s = v.get<std::string>();
                if (s == "horizontal") elf->HorizontalStack();
                else if (s == "vertical") elf->VerticalStack();
            }
        },

        // Misc
        {"invisible", [](UIFluentAPI *elf, const json &v) { elf->InvisibleButOccupySpace(v.get<bool>()); }},
    };
}
