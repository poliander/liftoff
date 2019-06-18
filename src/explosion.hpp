#ifndef EXPLOSION_HPP_
#define EXPLOSION_HPP_

#include "entity.hpp"
#include "particles.hpp"

class Explosion : public Entity
{
    public:
        Explosion(unsigned short int type, float x, float y, float z);
        ~Explosion();

        void            move(State &s);
        void            draw(State &s);

    private:
        ParticleEngine* particles;

        bool            has_particles;
        float           timer;
};

#endif
