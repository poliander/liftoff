#pragma once

#include "entity.hpp"
#include "explosion.hpp"

class Missile : public Entity
{
    public:
        Missile(State &s, short p);
        ~Missile();

        void update();
        void draw();

    protected:
        int  power;
        void collide(shared_ptr<Entity> e);
};
