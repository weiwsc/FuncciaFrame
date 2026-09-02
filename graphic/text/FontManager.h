//
// Created by Wangsicong Wei on 2026-01-11.
//

#ifndef FONTMANAGER_H
#define FONTMANAGER_H
#include <string>

#include "../../core/Singleton.h"

namespace vva::UI {
    class FontManager : public core::Singleton<FontManager>{
    public:
        auto GetFont() -> std::string&{
            return m_font;
        }
    private:
        std::string m_font = "../graphic/assets/fonts/NotoSans-VariableFont_wdth,wght.ttf";
    };
}

#endif //FONTMANAGER_H