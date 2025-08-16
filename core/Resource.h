//
// Created by Wangsicong Wei on 2025-08-15.
//

#ifndef FUNCCIAFRAME_RESOURCE_H
#define FUNCCIAFRAME_RESOURCE_H

#include <iostream>

#include "ISerializable.h"
#include "Asset.h"
#include "ObjectPool.h"
#include "Serializer.h"
#include "TypeRegistry.h"

#include "util.h"

namespace Funccia::Core {
    using std::string, std::byte;

    class Resource : public ISerializable{
    public:
        Resource();
        virtual ~Resource();

        void Serialize(Serializer& serializer) override;

        void Deserialize(Deserializer& deserializer) override;
        virtual void AssignNonDefaultValues();
        virtual void ToString();

        virtual void Reset() {};

        virtual auto TypeID() -> TypeID {return TypeID::Resource;}


    protected:
        template<class T>
        void SerializePointer(std::ostream& _stream, T* _pointer) {
            byte exists {1} ;
            if (_pointer != nullptr) {
                FF_ASSERT(dynamic_cast<Resource*>(_pointer) != nullptr, "Tying to serialize unsupported class.");
                _stream.write(reinterpret_cast<char*>(&exists), sizeof(byte));
                _pointer->Serialize(_stream);
            }
            else {
                exists = std::byte{0};
                _stream.write(reinterpret_cast<char*>(&exists), sizeof(byte));
            }
        }
        template<class T>
        void DeserializePointer(std::istream& _stream, T*& _pointer) {
            byte exists{ 0 };
            _stream.read(reinterpret_cast<char*>(&exists), sizeof(exists));
            if (static_cast<int>(exists) == 1) {
                _pointer = T::Pool->GetResource();
                _pointer->Deserialize(_stream);
            }

        }
        void SerializeAsset(std::ostream& _stream, Asset* _asset);
        void DeserializeAsset(std::istream& _stream, Asset*& _asset);

    private:
        Asset* m_asset;
    };
}
#endif //FUNCCIAFRAME_RESOURCE_H