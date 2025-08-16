//
// Created by Wangsicong Wei on 2025-08-15.
//

#include "Resource.h"

#include "AssetController.h"
#include "Serializer.h"

namespace Funccia::Core {
    Resource::Resource() {
        m_asset = nullptr;
    }

    Resource::~Resource() {

    }

    void Resource::AssignNonDefaultValues() {
    }
    void Resource::ToString() {
        cout << "BASE RESOURCE" << endl;
    }
    void Resource::SerializeAsset(std::ostream& _stream, Asset* _asset) {
        byte guidLength{ 0 };
        if (_asset != nullptr) {
            guidLength = static_cast<byte>(_asset->GetGUID().length());
        }
        _stream.write(reinterpret_cast<char*>(&guidLength), sizeof(guidLength));
        if (static_cast<int>(guidLength) > 0) {

            _stream.write(_asset->GetGUID().c_str(), static_cast<int>(guidLength));
        }
    }
    void Resource::DeserializeAsset(std::istream& _stream, Asset*& _asset) {
        byte guidLength{ 0 };
        _stream.read(reinterpret_cast<char*>(&guidLength), sizeof(byte));
        if (static_cast<int>(guidLength) > 0) {
            char guid[255];
            _stream.read(guid, static_cast<int>(guidLength));
            guid[static_cast<int>(guidLength)] = 0; //null terminate
            _asset = AssetController::Instance().GetAsset(string(guid));
        }
    }


    void Resource::Serialize(Serializer& serializer) {

    }

    void Resource::Deserialize(Deserializer &deserializer) {
    }


}
