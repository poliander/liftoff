#pragma once

#include "entity.hpp"

class Debris : public Entity {
    public:
        Debris(State &s);
        ~Debris();

        void update();
};
