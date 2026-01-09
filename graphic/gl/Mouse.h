//
// Created by Wangsicong Wei on 2026-01-08.
//

#ifndef MOUSE_H
#define MOUSE_H
#include <glm/vec2.hpp>
#include <SDL3/SDL_events.h>

namespace Funccia::Graphic::GL {
    class Mouse
    {
    public:
        Mouse();
        virtual ~Mouse();

        glm::vec2 GetPos();
        glm::vec2 GetPosPrev() { return m_posPrev; }
        bool GetButLDown() { return m_butLDown; }
        bool GetButMDown() { return m_butMDown; }
        bool GetButRDown() { return m_butRDown; }

        bool Moved(SDL_Event _event,  glm::vec2 _pos);
        void ProcessButtons(SDL_Event _event);
    private:
        glm::vec2 m_pos {0,0};
        glm::vec2 m_posPrev {0,0};
        bool m_butLDown;
        bool m_butMDown;
        bool m_butRDown;
    };
}

#endif //