//
// Created by Wangsicong Wei on 2025-09-29.
//

#ifndef FUNCCIAFRAME_UIHELPER_H
#define FUNCCIAFRAME_UIHELPER_H
#include <algorithm>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Funccia::UI {
    using vec4 = glm::vec4;
    using vec2 = glm::vec2;
    class UIHelper {
    public:
        static vec4 intersect(vec4 a, vec4 b) {
            return {
                std::max(a.x, b.x),  // left: max of left edges
                std::max(a.y, b.y),  // top: max of top edges
                std::min(a.z, b.z),  // right: min of right edges
                std::min(a.w, b.w)   // bottom: min of bottom edges
            };
        }
        static auto point_intersect(vec4 a, vec2 b) -> bool {
            return a.x <= b.x && a.z >= b.x && a.y <= b.y && a.w >= b.y;
        }
        static bool overlaps(vec4 a, vec4 b) {
            return !(a.x >= b.z || a.z <= b.x || a.y >= b.w || a.w <= b.y);
        }
    };
}
#endif //FUNCCIAFRAME_UIHELPER_H