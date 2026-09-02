//
// Created by Wangsicong Wei on 2025-09-27.
//

#include "UIElement.h"
namespace vva::UI {
    auto UIElement::AddChild(Tag tag)->UIElement* {
        auto child = std::make_unique<UIElement>(tag);
        return AddChild(std::move(child));
    }

    UIElement::UIElement(Tag tag) {

    }

}