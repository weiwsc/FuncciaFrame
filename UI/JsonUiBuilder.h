//
// Created by Wangsicong Wei on 2026-01-08.
//

#ifndef JSON_UI_BUILDER_H
#define JSON_UI_BUILDER_H
#include "SideOffset.h"
#include <string>

#include "UIElement.h"
#include <nlohmann/json.hpp>

namespace Funccia::UI {
    using json = nlohmann::json;
    using StyleHandler = std::function<void(UIFluentAPI*, const json&)>;

    class JsonUiBuilder {
    public:
        static auto ParseElement(const json& j, UIElement* el)-> void;
        static auto HandleStyle(nlohmann::basic_json<> styles, UIElement *root) -> void;
        static auto ParseColor(const json& v) -> glm::vec4;
        static auto ParseUI(const std::string& jsonStr, UIElement* root) -> void;
        static vec2 parseVec2(const json& v) {
            if (v.is_array() && v.size() == 2) {
                return {v[0], v[1]};
            }
            return {0, 0};
        }

        static const std::unordered_map<std::string, StyleHandler> styleHandlers;

    };
}

#endif //
