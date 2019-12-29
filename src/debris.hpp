#pragma once

#include "entity.hpp"

class Debris : public Entity {
    public:
        explicit Debris(State& s);
        ~Debris();

        void update();
};
