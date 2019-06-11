#ifndef MISSILE_HPP_
#define MISSILE_HPP_

#include "definitions.hpp"
#include "entity.hpp"
#include "state.hpp"

class Missile : public Entity
{
    public:
        Missile();
        ~Missile();

        void move(State &s);
        void draw(State &s);
};

#endif
