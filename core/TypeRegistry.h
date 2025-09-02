//
// Created by Wangsicong Wei on 2025-08-15.
//

#ifndef FUNCCIAFRAME_TYPEREGISTRY_H
#define FUNCCIAFRAME_TYPEREGISTRY_H

namespace Funccia::Core {
    class ISerializable;
    enum class TypeID {
        None = 0,
        Asset = 1,
        Resource = 2,
        ContainerA = 3,
        Object1 = 4,
        Object2 = 5,
    };
    class TypeRegistry {
    public:
        static auto GetTypeDefaultObject(TypeID _typeID) -> ISerializable*;

    };
}


#endif //FUNCCIAFRAME_TYPEREGISTRY_H