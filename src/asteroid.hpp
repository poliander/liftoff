#ifndef ASTEROID_HPP_
#define ASTEROID_HPP_

#include "entity.hpp"
#include "explosion.hpp"

class Asteroid : public Entity
{
    public:
        Asteroid();
        ~Asteroid();

        bool damage(State &s, int p);

        void move(State &s);
        void draw(State &s);
};

#endif
