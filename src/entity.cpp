#include "entity.hpp"
#include "state.hpp"

Entity::Entity(State &s) : state(s)
{
}

Entity::~Entity()
{
}
