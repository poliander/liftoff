#pragma once

#include "entity.hpp"
#include "explosion.hpp"

class State;

class Missile : public Entity
{
    public:
        Missile(short p);
        ~Missile();

        void update(State &s);
        void draw(State &s);

    protected:
        int  power;
        void collide(State &s, shared_ptr<Entity> e);
};
