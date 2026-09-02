//
// Created by Wangsicong Wei on 2025-10-10.
//

#ifndef FUNCCIAFRAME_IRECTPACKER_H
#define FUNCCIAFRAME_IRECTPACKER_H
namespace vva::gfx {
    class IRectPacker {
    public:
        virtual ~IRectPacker() = default;
        auto virtual Pack(int width, int height, int &x, int &y)->bool = 0;
    };
}
#endif //FUNCCIAFRAME_IRECTPACKER_H