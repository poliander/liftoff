#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "state.hpp"

class Entity
{
    public:
        Entity(State &s);
        ~Entity();

    private:
        State& state;
};

#endif
