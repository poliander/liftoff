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
        float           j_l;              // jet axhaust animation
        float           jt_l;
        float           jr;

        unsigned short  m_alt;
        GLuint          m_next_shot;

        float           gun_flash[2];     // gun flashes
        float           gun_flash_rot[2];
        unsigned short  gun_power;
};
