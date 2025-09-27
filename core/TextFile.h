//
// Created by Wangsicong Wei on 2025-09-05.
//

#ifndef FUNCCIAFRAME_TEXTFILE_H
#define FUNCCIAFRAME_TEXTFILE_H
#include "Resource.h"

namespace Funccia::Core {
    class TextFile : Resource{
    public:
        TextFile();
        virtual ~TextFile();
        void Serialize(Serializer &serializer) override;

        void Deserialize(Deserializer &deserializer) override;

        auto TypeID() -> Core::TypeID override;

    private:
        Asset* m_asset;
    };
}

#endif //FUNCCIAFRAME_TEXTFILE_H