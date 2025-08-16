//
// Created by Wangsicong Wei on 2025-08-14.
//

#ifndef FUNCCIAFRAME_STACKALLOCATOR_H
#define FUNCCIAFRAME_STACKALLOCATOR_H
#include <cstddef>


namespace Funccia::Core {
    using std::byte;

    class StackAllocator {
    public:
        StackAllocator();
        virtual ~StackAllocator();

        [[nodiscard]]
        auto GetMarker() const -> byte * { return m_marker; }

        //size_t StackTotalSizeBytes() const { return m_stackEnd - m_stackStart; }

        [[nodiscard]]
        auto GetUsedSizeBytes() const -> std::size_t { return m_stackPosition - m_stackStart; }

        //size_t GetAvailableSizeBytes() const { return m_stackEnd - m_stackPosition; }

        void AllocateStack_Bytes(size_t _stackSizeBytes);

        void AllocateStack_KiloBytes(size_t _stackSizeKiloBytes);
        void AllocateStack_MegaBytes(size_t _stackSizeMegaBytes);

        auto GetMemory(size_t _sizeBytes) -> byte *;

        void Mark();

        //void FreeToMarker();

        void FreeToMarkerClean();

        void ClearMemory();

    private:
        byte* m_marker;
        byte* m_stackStart;
        byte* m_stackPosition;
        byte* m_stackEnd;

    };
}

#endif //FUNCCIAFRAME_STACKALLOCATOR_H