//
// Created by Wangsicong Wei on 2026-01-14.
//

#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

namespace Funccia::Graphic {
    class GraphicsDevice {
    public:
        virtual ~GraphicsDevice() = default;

        virtual void Init() = 0;
        virtual void Update() = 0;
    };
}

#endif //