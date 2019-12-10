#pragma once

#include "debris.hpp"
#include "entity.hpp"
#include "explosion.hpp"

class Asteroid : public Entity
{
    public:
        Asteroid();
        ~Asteroid();

        bool damage(State &s, int p);
        void update(State &s);
};
