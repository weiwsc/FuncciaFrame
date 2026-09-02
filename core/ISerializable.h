//
// Created by Wangsicong Wei on 2025-08-15.
//

#ifndef FUNCCIAFRAME_SERIALIZABLE_H
#define FUNCCIAFRAME_SERIALIZABLE_H

#include <iostream>



namespace vva::core {
    class Deserializer;
    class Serializer;

    class ISerializable {
    public:
        ISerializable() = default;
        virtual ~ISerializable() = default;

        virtual void Serialize(Serializer& serializer) = 0;
        virtual void Deserialize(Deserializer& deserializer) = 0;
    };
}

#endif //FUNCCIAFRAME_SERIALIZABLE_H