//
// Created by Wangsicong Wei on 2025-08-15.
//

#include "TypeRegistry.h"
#include "Resource.h"
#include "ObjectPoolManager.h"

auto Funccia::Core::TypeRegistry::GetTypeDefaultObject(TypeID _typeID) -> Resource * {
    switch (_typeID) {
            case TypeID::Asset: return dynamic_cast<Resource *>(ObjectPoolManager::AssetPool->GetResource());
            case TypeID::Resource: return dynamic_cast<Resource *>(ObjectPoolManager::ResourcePool->GetResource());
            case TypeID::ContainerA: return dynamic_cast<Resource *>(ObjectPoolManager::ContainerAPool->GetResource());
            case TypeID::Object1: return dynamic_cast<Resource *>(ObjectPoolManager::Object1Pool->GetResource());
            case TypeID::Object2: return dynamic_cast<Resource *>(ObjectPoolManager::Object2Pool->GetResource());

    }
}
