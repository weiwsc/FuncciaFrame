//
// Created by Wangsicong Wei on 2025-10-10.
//

#ifndef FUNCCIAFRAME_GRIDRECTPACKER_H
#define FUNCCIAFRAME_GRIDRECTPACKER_H
#include "IRectPacker.h"

namespace vva::gfx {
    class GridRectPacker : public IRectPacker {
    public:
        GridRectPacker(int atlasHeight, int atlasWidth,int pad, int maxLayer);
        ~GridRectPacker() override;

        auto Pack(int width, int height, int &x, int &y)->bool override;
    private:
        auto PackInRow(int paddedWidth, int paddedHeight, int &x, int &y) -> bool;
        auto AdvanceRow() -> bool;
        auto AdvanceLayer() -> bool;

        int m_pad{0};
        int m_maxLayer{0};
        int m_cursorX{0};
        int m_cursorY{0};
        int m_maxRowHeight{0};
        int m_atlasWidth{0};
        int m_atlasHeight{0};
        int m_maxAtlasLayers{0};
        int m_atlasCurrentLayerIndex{0};
    };
}
#endif //FUNCCIAFRAME_GRIDRECTPACKER_H