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

bool Entity::isFocusable()
{
    return focusable;
}

void Entity::activate()
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

void Entity::setScale(float x, float y, float z)
{
    s_x = x;
    s_y = y;
    s_z = z;
}

float Entity::getScaleX()
{
    return s_x;
}

float Entity::getScaleY()
{
    return s_y;
}

float Entity::getScaleZ()
{
    return s_z;
}

void Entity::setSpin(float x, float y, float z)
{
    w_x = x;
    w_y = y;
    w_z = z;
}

int Entity::getLife()
{
    return life;
}

void Entity::setLife(int l)
{
    life = l;
}

bool Entity::isColliding(shared_ptr<Entity> e)
{
    float s1 = (10000.0f + p_z) * .0005f * max(max(s_x, s_y), s_z);
    float s2 = (10000.0f + e->getPosZ()) * .0005f * max(max(e->getScaleX(), e->getScaleY()), e->getScaleZ());

    float d_x = e->getPosX() - p_x;
    float d_y = e->getPosY() - p_y;
    float d_z = e->getPosZ() - p_z;

    return ((1.0f / isqrt(d_x * d_x + d_y * d_y + d_z * d_z)) < (s1 + s2));
}

void Entity::collide(State &s, shared_ptr<Entity> e)
{
}

void Entity::damage(State &s, int p)
{
    life -= p;

    if (life <= 0) {
        e_state = OBJ_STATE_GONE;
    }
}

void Entity::move(State &s)
{
    p_x += s.timer_adjustment * v_x;
    p_y += s.timer_adjustment * v_y;
    p_z += s.timer_adjustment * (v_z + E_BASE_SPEED);

    r_x += s.timer_adjustment * w_x;
    if (r_x < 0) r_x += 360.0f;
    if (r_x > 360.0f) r_x -= 360.0f;

    r_y += s.timer_adjustment * w_y;
    if (r_y < 0) r_y += 360.0f;
    if (r_y > 360.0f) r_y -= 360.0f;

    r_z += s.timer_adjustment * w_z;
    if (r_z < 0) r_z += 360.0f;
    if (r_z > 360.0f) r_z -= 360.0f;
}
