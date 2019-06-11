#include "entity.hpp"

Entity::Entity()
{
}

Entity::~Entity()
{
}

bool Entity::isCollider()
{
    return e_type == OBJ_TYPE_COLLIDER;
}

bool Entity::isIdle()
{
    return e_state == OBJ_STATE_IDLE;
}

bool Entity::isGone()
{
    return e_state == OBJ_STATE_GONE;
}

void Entity::setActive()
{
    e_state = OBJ_STATE_ACTIVE;
}

void Entity::setPos(float x, float y, float z)
{
    p_x = x;
    p_y = y;
    p_z = z;
}

void Entity::setPosX(float x)
{
    p_x = x;
}

void Entity::setPosY(float y)
{
    p_y = y;
}

void Entity::setPosZ(float z)
{
    p_z = z;
}

float Entity::getPosX()
{
    return p_x;
}

float Entity::getPosY()
{
    return p_x;
}

float Entity::getPosZ()
{
    return p_z;
}
