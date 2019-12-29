#pragma once

#include <cmath>
#include <memory>

#include <glm/glm.hpp>

#include "entity.hpp"
#include "missile.hpp"

class Player : public Entity
{
    public:
        Player(State &s);
        ~Player();

        void            init();
        void            update();
        void            draw();
        void            shoot();
        void            collide(shared_ptr<Entity> e);
        void            collect(uint16_t e_obj);

    private:
        uint8_t         m_alt;
        Uint32          m_next_shot_timer;
        Uint32          powerup_timer;

        float           gun_flash[2];
        float           gun_flash_rot[2];
};
