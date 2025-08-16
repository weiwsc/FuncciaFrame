//
// Created by Wangsicong Wei on 2025-08-14.
//

#include "StackAllocator.h"

#include <cstring>

#include "util.h"

namespace Funccia::Core {
    #define B_PER_MB 1048576
    #define B_PER_KB 1024

    StackAllocator::StackAllocator() :m_stackStart(nullptr), m_stackPosition(nullptr), m_stackEnd(nullptr), m_marker(nullptr) {}
    StackAllocator::~StackAllocator() {
        ClearMemory();
    }
    void StackAllocator::AllocateStack_Bytes(size_t _stackSizeBytes) {
        ClearMemory();
        m_stackStart = new byte[_stackSizeBytes];
        memset(m_stackStart, 0, _stackSizeBytes);
        m_stackPosition = m_stackStart;
        m_stackEnd = m_stackStart + _stackSizeBytes;
    }

    void StackAllocator::AllocateStack_KiloBytes(size_t _stackSizeKiloBytes) {
        AllocateStack_Bytes(_stackSizeKiloBytes * B_PER_KB);
    }
    void StackAllocator::AllocateStack_MegaBytes(size_t _stackSizeMegaBytes) {
        AllocateStack_Bytes(_stackSizeMegaBytes * B_PER_MB);
    }

    byte* StackAllocator::GetMemory(size_t _sizeBytes) {
        FF_ASSERT(m_stackStart != nullptr, "No memory is allocated for StackAllocator yet. Run AllocateStack first.");
        byte* stackPositionOrig = m_stackPosition;
        if (m_stackPosition + _sizeBytes <= m_stackEnd) {
            m_stackPosition += _sizeBytes;
            return stackPositionOrig;
        }
        return nullptr; // not enough bytes left on stack
    }
    void StackAllocator::Mark() {
        m_marker = m_stackPosition;
    }

    /**
     *It only moves stackPosition to Marker. This will not reset the memory to 0.
     *To reset memory as well, use FreeToMarkerClean
     */

    /*
    void StackAllocator::FreeToMarker() {
        FF_ASSERT(m_marker != nullptr && m_marker >= m_stackStart && m_marker <= m_stackEnd, "Invalid marker or invalid marker position");
        m_stackPosition = m_marker;
    }
    */
    void StackAllocator::FreeToMarkerClean() {
        FF_ASSERT(m_marker != nullptr && m_marker >= m_stackStart && m_marker <= m_stackEnd, "Invalid marker or invalid marker position");
        m_stackPosition = m_marker;
        memset(m_stackPosition, 0, m_stackEnd - m_stackPosition);
    }
    void StackAllocator::ClearMemory() {
        if (m_stackStart != nullptr) {
            delete[] m_stackStart;
        }
        m_marker = nullptr;
        m_stackStart = nullptr;
        m_stackPosition = nullptr;
        m_stackEnd = nullptr;
    }





}
