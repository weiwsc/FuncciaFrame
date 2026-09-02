//
// Created by Wangsicong Wei on 2025-10-10.
//

#include "GridRectPacker.h"

#include <algorithm>

namespace vva::gfx {

    auto GridRectPacker::Pack(int width, int height, int &x, int &y) -> bool {
        const int pw = width + 2 * m_pad;
        const int ph = height + 2 * m_pad;

        while (!PackInRow(pw, ph, x, y)) {
            while (!AdvanceRow()) {
                if (!AdvanceLayer()) {
                    return false;
                }
            }
        }
        x += m_pad;
        y += m_pad;
        return true;
    }


    GridRectPacker::GridRectPacker(int atlasHeight, int atlasWidth, int pad, int maxLayer) {
    }

    GridRectPacker::~GridRectPacker() {
    }

    auto GridRectPacker::PackInRow(int paddedWidth, int paddedHeight, int &x, int &y) -> bool {
        if (m_cursorY + paddedHeight > m_atlasHeight) return false;
        if (m_cursorX + paddedWidth <= m_atlasWidth) {
            x = m_cursorX;
            y = m_cursorY;
            m_cursorX += paddedWidth;
            m_maxRowHeight = std::max(m_maxRowHeight, paddedHeight);
            return true;
        }
        return false;
    }

    auto GridRectPacker::AdvanceRow() -> bool {
        if (m_cursorY + m_maxRowHeight < m_atlasHeight) {
            m_cursorX = m_pad;
            m_cursorY += m_maxRowHeight;
            m_maxRowHeight = 0;
            return true;
        }
        return false;
    }

    auto GridRectPacker::AdvanceLayer() -> bool {
        if (m_atlasCurrentLayerIndex + 1 >= m_maxAtlasLayers) {
            return false; // no more pages available
        }
        m_atlasCurrentLayerIndex += 1;

        m_cursorX = m_pad;
        m_cursorY = m_pad;
        m_maxRowHeight = 0;
        return true;
    }
}
