//
// Created by Wangsicong Wei on 2025-08-14.
//

#ifndef FUNCCIAFRAME_ASSETCONTROLLER_H
#define FUNCCIAFRAME_ASSETCONTROLLER_H
#include <map>

#include "Asset.h"
#include "Singleton.h"
#include "StackAllocator.h"

namespace Funccia::Core {
    class AssetController : public Singleton<AssetController> {
    public:
        AssetController();
        virtual ~AssetController();

        void Initialize(int _stackSize);
        void Clear();
        auto GetAsset(string _guid) -> Asset *;

        static StackAllocator* Stack;

    private:
        std::map<string, Asset*> m_assets;
    };
}
#endif //FUNCCIAFRAME_ASSETCONTROLLER_H