//
// Created by Wangsicong Wei on 2025-09-05.
//

#ifndef FUNCCIAFRAME_TEXTFILE_H
#define FUNCCIAFRAME_TEXTFILE_H
#include "Resource.h"

namespace vva::core {
    class TextFile : Resource{
    public:
        TextFile();
        virtual ~TextFile();
        void Serialize(Serializer &serializer) override;

        void Deserialize(Deserializer &deserializer) override;

        auto TypeID() -> core::TypeID override;

    private:
        Asset* m_asset;
    };
}

#endif //FUNCCIAFRAME_TEXTFILE_H