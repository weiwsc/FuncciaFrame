//
// Created by Wangsicong Wei on 2025-08-14.
//

#include "Asset.h"
#include "util.h"

#include <iostream>

namespace vva::core {
    Asset::Asset() {
        m_GUID = "";
        m_dataSize = 0;
        m_data = nullptr;
    }
    void Asset::Reset() {
        m_GUID = "";
        m_dataSize = 0;
        m_data = nullptr;
    }
    void Asset::ToString() {
        std::cout << "Asset GUID: " << m_GUID << std::endl;
        std::cout << "Asset Data Size: " << m_dataSize <<" bytes, "<< Utilities::toReadableSize(m_dataSize) << std::endl;
    }
}
