//
// Created by Wangsicong Wei on 2025-08-15.
//

#include "TypeRegistry.h"
#include "ISerializable.h"
#include "ObjectPoolManager.h"

auto Funccia::Core::TypeRegistry::GetTypeDefaultObject(TypeID _typeID) -> ISerializable * {
    switch (_typeID) {
            case TypeID::Resource: return dynamic_cast<ISerializable *>(ObjectPoolManager::ResourcePool->GetResource());
            case TypeID::ContainerA: return dynamic_cast<ISerializable *>(ObjectPoolManager::ContainerAPool->GetResource());
            case TypeID::Object1: return dynamic_cast<ISerializable *>(ObjectPoolManager::Object1Pool->GetResource());
            case TypeID::Object2: return dynamic_cast<ISerializable *>(ObjectPoolManager::Object2Pool->GetResource());

    }
}
