#pragma once

#include "entity.hpp"
#include "particles.hpp"

class Explosion : public Entity
{
    public:
        Explosion(State &s, short int type, float x, float y, float z);
        Explosion(State &s, short int type, float x, float y, float z, float r, float g, float b);
        ~Explosion();

        void                       update();
        void                       draw();

    private:
        unique_ptr<ParticleEngine> particles;
};
