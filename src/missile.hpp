#ifndef MISSILE_HPP_
#define MISSILE_HPP_

#include "entity.hpp"

class State;

class Missile : public Entity
{
    public:
        Missile();
        ~Missile();

        void         move(State &s);
        void         draw(State &s);

    protected:
        int          power;

        void         collide(State &s, shared_ptr<Entity> e);
};

#endif
