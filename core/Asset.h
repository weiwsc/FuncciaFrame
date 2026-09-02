//
// Created by Wangsicong Wei on 2025-08-14.
//

#ifndef FUNCCIAFRAME_ASSET_H
#define FUNCCIAFRAME_ASSET_H
#include <cstddef>
#include <string>
#include "ObjectPool.h"

namespace vva::core {
    using std::string, std::byte;

    class Asset{
    public:
        Asset();
        virtual ~Asset() {}

        auto GetGUID() -> string & { return m_GUID; }
        void SetGUID(const string &_guid) { m_GUID = _guid; }

        auto GetDataSize() -> size_t { return m_dataSize; }
        void SetDataSize(size_t _size) { m_dataSize = _size; }

        auto GetData() const -> byte * { return m_data; }
        void SetData(byte* _data) { m_data = _data; }

        void Reset();
        void ToString();

    private:
        string m_GUID;
        size_t m_dataSize;
        byte* m_data;
    };
}

#endif //FUNCCIAFRAME_ASSET_H