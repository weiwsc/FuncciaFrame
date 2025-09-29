//
// Created by Wangsicong Wei on 2025-09-27.
//

#include "UIElement.h"
namespace Funccia::UI {
    auto UIElement::AddChild(Tag tag)->UIElement* {
        auto child = std::make_unique<UIElement>(tag);
        return AddChild(std::move(child));
    }

    UIElement::UIElement(Tag tag) {
        SetTag(tag);
        switch (tag) {
            case Tag::Paragraph:
                HorizontalGrow();
                VerticalGrow();
                break;
            case Tag::Label:
                HorizontalGrow();
                VerticalFit();   //?????
                break;
            case Tag::AutoMargin:
                InvisibleButOccupySpace(true);
                HorizontalGrow();
                VerticalGrow();
                break;
            default:
                this->SetTag(Tag::None)
                ;
        }

    }

}