#pragma once

#include <memory>

#include "entity.hpp"
#include "explosion.hpp"
#include "powerup.hpp"

class Cargo : public Entity {
    public:
        explicit Cargo(State* s);
        ~Cargo();

        bool  damage(int p);
        void  update();
        void  draw();

    private:
        float counter = 0;
        bool  yield   = false;
};
