#pragma once

#include "entity.hpp"

class Debris : public Entity
{
    public:
        Debris();
        ~Debris();

        void move(State &s);
        void draw(State &s);
};
