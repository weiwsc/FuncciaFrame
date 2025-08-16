//
// Created by Wangsicong Wei on 2025-08-15.
//

#include "ObjectPoolManager.h"

namespace Funccia::Core {
    ObjectPool<Asset>* ObjectPoolManager::AssetPool;
    ObjectPool<Resource>* ObjectPoolManager::ResourcePool;

    ObjectPool<ContainerA>* ObjectPoolManager::ContainerAPool;
    ObjectPool<Object1>* ObjectPoolManager::Object1Pool;
    ObjectPool<Object2>* ObjectPoolManager::Object2Pool;
}