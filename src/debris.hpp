#ifndef DEBRIS_HPP_
#define DEBRIS_HPP_

#include "entity.hpp"

class Debris : public Entity
{
    public:
        Debris();
        ~Debris();

        void move(State &s);
        void draw(State &s);
};

#endif
