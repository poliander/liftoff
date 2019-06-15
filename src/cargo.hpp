#ifndef CARGO_HPP_
#define CARGO_HPP_

#include "entity.hpp"

class Cargo : public Entity
{
    public:
        Cargo();
        ~Cargo();

        bool damage(State &s, int p);

        void move(State &s);
        void draw(State &s);

    private:
        float counter = 0;
};

#endif
