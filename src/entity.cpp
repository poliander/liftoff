#include "entity.hpp"

Entity::Entity()
{
}

Entity::~Entity()
{
}

bool Entity::sort(const shared_ptr<Entity> &e1, const shared_ptr<Entity> &e2)
{
    return (e1->getPosZ() < e2->getPosZ());
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

bool Entity::isAlive()
{
    return life > 0;
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
    return p_y;
}

float Entity::getPosZ()
{
    return p_z;
}

void Entity::setVelocity(float x, float y, float z)
{
    v_x = x;
    v_y = y;
    v_z = z;
}

float Entity::getVelocityX()
{
    return v_x;
}

float Entity::getVelocityY()
{
    return v_x;
}

float Entity::getVelocityZ()
{
    return v_z;
}

void Entity::setScale(float x, float y, float z)
{
    s_x = x;
    s_y = y;
    s_z = z;
}

float Entity::getScale()
{
    return .333f * (s_x +  s_y + s_z);
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

void Entity::setAcceleration(float x, float y, float z)
{
    a_x = x;
    a_y = y;
    a_z = z;
}

void Entity::setAccelerationX(float x)
{
    a_x = x;
}

void Entity::setAccelerationY(float y)
{
    a_y = y;
}

void Entity::setAccelerationZ(float z)
{
    a_z = z;
}

void Entity::setRotation(float x, float y, float z)
{
    r_x = x;
    r_y = y;
    r_z = z;
}

float Entity::getRotationX()
{
    return r_x;
}

float Entity::getRotationY()
{
    return r_y;
}

float Entity::getRotationZ()
{
    return r_z;
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

bool Entity::isColliding(State &s, shared_ptr<Entity> e)
{
    float ds, r1, r2, x1, y1, z1, x2, y2, z2;
    
    r1 = e->getScale();
    r1 = r1 * (10000.0f + p_z) * .0001f;

    r2 = e->getScale();
    r2 = r2 * (10000.0f + e->getPosZ()) * .0001f;

    x1 = E_RELATIVE_MOVEMENT * ((p_x + s.timer_adjustment * v_x) - s.cam_x);
    y1 = E_RELATIVE_MOVEMENT * ((p_y + s.timer_adjustment * v_y) - s.cam_y);
    z1 = p_z + s.timer_adjustment * (v_z + E_BASE_SPEED);

    x2 = E_RELATIVE_MOVEMENT * ((e->getPosX() + s.timer_adjustment * e->getVelocityX()) - s.cam_x);
    y2 = E_RELATIVE_MOVEMENT * ((e->getPosY() + s.timer_adjustment * e->getVelocityY()) - s.cam_y);
    z2 = e->getPosZ() + s.timer_adjustment * e->getVelocityZ();

    ds = pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2);

    return ((1.0f / isqrt(ds)) < (r1 + r2));
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

    r_x += s.timer_adjustment * w_x * .1f;
    if (r_x < 0) r_x += 360.0f;
    if (r_x > 360.0f) r_x -= 360.0f;

    r_y += s.timer_adjustment * w_y * .1f;
    if (r_y < 0) r_y += 360.0f;
    if (r_y > 360.0f) r_y -= 360.0f;

    r_z += s.timer_adjustment * w_z * .1f;
    if (r_z < 0) r_z += 360.0f;
    if (r_z > 360.0f) r_z -= 360.0f;
}
