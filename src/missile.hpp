#pragma once

#include <memory>

#include "entity.hpp"
#include "explosion.hpp"

class Missile : public Entity {
    public:
        Missile(State &s, uint16_t p);
        ~Missile();

        void     collide(shared_ptr<Entity> e);
        void     update();
        void     draw();

    protected:
        uint16_t power;
};
