#pragma once

#include <cmath>

#include <glm/glm.hpp>

#include "entity.hpp"
#include "missile.hpp"

class Player : public Entity
{
    public:
        Player();
        ~Player();

        void            update(State &s);
        void            draw(State &s);
        void            shoot(State &s);
        void            init(State &s);
        void            collide(State &s, shared_ptr<Entity> e);
        void            collect(unsigned short e_obj);

    private:
        unsigned short  m_alt;
        GLuint          m_next_shot;

        float           gun_flash[2];
        float           gun_flash_rot[2];

        Uint32          tick_timer;
};
