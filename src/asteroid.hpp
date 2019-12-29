#pragma once

#include <memory>

#include "debris.hpp"
#include "entity.hpp"
#include "explosion.hpp"

class Asteroid : public Entity {
    public:
        explicit Asteroid(State& s);
        ~Asteroid();

        bool damage(int p);
        void update();
};
