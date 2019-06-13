#ifndef ASTEROID_HPP_
#define ASTEROID_HPP_

#include "entity.hpp"

class State;

class Asteroid : public Entity
{
    public:
        Asteroid();
        ~Asteroid();

        void move(State &s);
        void draw(State &s);
};

#endif
