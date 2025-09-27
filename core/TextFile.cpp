//
// Created by Wangsicong Wei on 2025-09-05.
//

#include "TextFile.h"
namespace Funccia::Core {
    TextFile::TextFile() {
        m_asset = nullptr;
    }

    TextFile::~TextFile() {
    }

    void TextFile::Serialize(Serializer &serializer) {
        serializer.write(m_asset);
        Resource::Serialize(serializer);
    }

    void TextFile::Deserialize(Deserializer &deserializer) {
        deserializer.read(m_asset);
        Resource::Deserialize(deserializer);
    }

    auto TextFile::TypeID() -> Core::TypeID {
        return TypeID::TextFile;
    }
}