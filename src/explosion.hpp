#pragma once

#include "entity.hpp"
#include "particles.hpp"

class Explosion : public Entity
{
    public:
        Explosion(short int type, float x, float y, float z);
        Explosion(short int type, float x, float y, float z, float r, float g, float b);
        ~Explosion();

        void                       update(State &s);
        void                       draw(State &s);

    private:
        unique_ptr<ParticleEngine> particles;
};
