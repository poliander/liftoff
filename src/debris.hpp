#pragma once

#include "entity.hpp"

class Debris : public Entity
{
    public:
        Debris();
        ~Debris();

        void update(State &s);
        void draw(State &s);
};
