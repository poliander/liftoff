#pragma once

#include "entity.hpp"
#include "message.hpp"
#include "particles.hpp"

class Powerup : public Entity
{
    public:
        Powerup(State &s, float x, float y, float z);
        ~Powerup();

        bool            damage(int p);
        void            collide(shared_ptr<Entity> e);
        void            update();
        void            draw();

    private:
        unique_ptr<ParticleEngine> particles;
        float                      counter;
};
