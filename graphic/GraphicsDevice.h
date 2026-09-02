//
// Created by Wangsicong Wei on 2026-01-14.
//

#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include "RenderTypes.h"

namespace vva::gfx {
    class WindowInterface;

    class GraphicsDevice {
    public:
        virtual ~GraphicsDevice() = default;

        virtual void Init(WindowInterface& window) = 0;
        virtual void BeginFrame(const FrameContext& frameContext) = 0;
    };
}

#endif //
