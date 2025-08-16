//
// Created by Wangsicong Wei on 2025-08-14.
//

#include "AssetController.h"
#include "ObjectPool.h"
#include "FileController.h"
#include <vector>

#include "ObjectPoolManager.h"

namespace Funccia::Core {
    StackAllocator* AssetController::Stack = nullptr;

    AssetController::AssetController() {

    }

    AssetController::~AssetController() {
        m_assets.clear();
    }

    void AssetController::Initialize(int _stackSizeBytes) {
        Stack = new StackAllocator();
        AssetController::Stack->AllocateStack_Bytes(_stackSizeBytes);
        ObjectPoolManager::AssetPool = new ObjectPool<Asset>();
    }

    void AssetController::Clear() {
        for (auto const& x : m_assets) {
            ObjectPoolManager::AssetPool->ReleaseResource(x.second);
        }
        if (ObjectPoolManager::AssetPool != nullptr) {
            delete ObjectPoolManager::AssetPool;
            ObjectPoolManager::AssetPool = nullptr;
        }
        delete Stack; //if only ClearMeomory() is called here it will cause memory leak if the AssetController is Initialized again.
        m_assets.clear();
    }

    auto AssetController::GetAsset(string _guid) -> Asset * {
        if (m_assets.count(_guid) != 0) {
            return m_assets[_guid];
        }

        Asset* asset = ObjectPoolManager::AssetPool->GetResource();
        asset->SetGUID(_guid);
        asset->SetDataSize(FileController::Instance().GetFileSize(_guid));
        asset->SetData(Stack->GetMemory(asset->GetDataSize()));
        FileController::Instance().ReadFile(_guid, asset->GetData(), asset->GetDataSize());

        m_assets[_guid] = asset;

        return asset;
    }
}
