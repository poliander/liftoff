#ifndef POWERUP_HPP_
#define POWERUP_HPP_

#include "entity.hpp"
#include "particles.hpp"

class Powerup : public Entity
{
    public:
        Powerup(float x, float y, float z);
        ~Powerup();

        bool damage(State &s, int p);

        void move(State &s);
        void draw(State &s);

    private:
        float           counter = 0;
        ParticleEngine* particles;
};

#endif
