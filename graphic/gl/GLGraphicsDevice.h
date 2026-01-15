//
// Created by Wangsicong Wei on 2026-01-14.
//

#pragma once
#include "../GraphicsDevice.h"

namespace Funccia::Graphic::GL
{
    class GLGraphicsDevice : public GraphicsDevice {
    public:
        ~GLGraphicsDevice() override = default;
        void Init() override;
        void Update() override;
    };
}


