//
// Created by Wangsicong Wei on 2025-08-15.
//

#ifndef FUNCCIAFRAME_OBJECTPOOLMANAGER_H
#define FUNCCIAFRAME_OBJECTPOOLMANAGER_H
#include "Asset.h"
#include "ObjectPool.h"
#include "Resource.h"
#include "Singleton.h"

namespace vva::core {
    class ObjectPoolManager : Singleton<ObjectPoolManager>{
    public:
        static ObjectPool<Asset>* AssetPool;
        static ObjectPool<Resource>* ResourcePool;

    };
}

#endif //FUNCCIAFRAME_OBJECTPOOLMANAGER_H