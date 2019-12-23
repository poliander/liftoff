#pragma once

#include "entity.hpp"
#include "message.hpp"
#include "particles.hpp"

class Powerup : public Entity
{
    public:
        Powerup(float x, float y, float z);
        ~Powerup();

        bool            damage(State &s, int p);
        void            collide(State &s, shared_ptr<Entity> e);
        void            update(State &s);
        void            draw(State &s);

    private:
        unique_ptr<ParticleEngine> particles;
        float                      counter;
};
