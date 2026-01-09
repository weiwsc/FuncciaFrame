//
// Created by Wangsicong Wei on 2026-01-08.
//

#include "Mouse.h"
namespace Funccia::Graphic::GL {
    Mouse::Mouse()
    {
        m_butLDown = false;
        m_butMDown = false;
        m_butRDown = false;
    }

    Mouse::~Mouse()
    {
    }

    glm::vec2 Mouse::GetPos() //relying on detecting mouse position based on sdl event mouse motion causes a bug
    //that the object snapping to mouse can snap back to 0,0 when mouse is not moved(seems like mouse pos is changed to 0,0 when it is not moved)
    {
        float x, y;
        SDL_GetMouseState(&x, &y);
        m_pos.x = x;
        m_pos.y = y;
        return m_pos;
    }

    bool Mouse::Moved(SDL_Event _event, glm::vec2 _pos)
    {
        if (_event.type != SDL_EVENT_MOUSE_MOTION) return false;

        m_pos.x = _event.motion.x;
        m_pos.y = _event.motion.y;
        _pos = m_pos;
        m_posPrev = m_pos;

        return true;
    }

    void Mouse::ProcessButtons(SDL_Event _event)
    {
        if (_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (_event.button.button == SDL_BUTTON_LEFT) m_butLDown = true;
            if (_event.button.button == SDL_BUTTON_MIDDLE) m_butMDown = true;
            if (_event.button.button == SDL_BUTTON_RIGHT) m_butRDown = true;
        }
        else if (_event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            if (_event.button.button == SDL_BUTTON_LEFT) m_butLDown = false;
            if (_event.button.button == SDL_BUTTON_MIDDLE) m_butMDown = false;
            if (_event.button.button == SDL_BUTTON_RIGHT) m_butRDown = false;
        }
    }
}