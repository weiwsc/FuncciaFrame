//
// Created by Wangsicong Wei on 2025-08-15.
//

#ifndef FUNCCIAFRAME_OBJECTPOOLMANAGER_H
#define FUNCCIAFRAME_OBJECTPOOLMANAGER_H
#include "Asset.h"
#include "ObjectPool.h"
#include "Resource.h"
#include "Singleton.h"
#include "../test/ContainerA.h"

namespace Funccia::Core {
    class ObjectPoolManager : Singleton<ObjectPoolManager>{
    public:
        static ObjectPool<Asset>* AssetPool;
        static ObjectPool<Resource>* ResourcePool;

        static ObjectPool<ContainerA>* ContainerAPool;
        static ObjectPool<Object1>* Object1Pool;
        static ObjectPool<Object2>* Object2Pool;
    };
}

#endif //FUNCCIAFRAME_OBJECTPOOLMANAGER_H