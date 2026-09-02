//
// Created by Wangsicong Wei on 2025-08-15.
//

#ifndef FUNCCIAFRAME_TYPEREGISTRY_H
#define FUNCCIAFRAME_TYPEREGISTRY_H

namespace vva::core {
    class ISerializable;
    enum class TypeID {
        None,
        Asset,
        Resource,
        TextFile,
        Shader,
        ContainerA,
        Object1,
        Object2,
    };
    class TypeRegistry {
    public:
        static auto GetTypeDefaultObject(TypeID _typeID) -> ISerializable*;

    };
}


#endif //FUNCCIAFRAME_TYPEREGISTRY_H