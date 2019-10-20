#pragma once

#include "entity.hpp"
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
        float           counter = 0;
        ParticleEngine* particles;
};
