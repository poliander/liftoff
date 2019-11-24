#include "entity.hpp"

Entity::Entity()
{
    money = 0;
    life = 1;
    focusable = false;
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
    return e_type == E_TYPE_COLLIDER;
}

bool Entity::isScenery()
{
    return e_type == E_TYPE_SCENERY;
}

bool Entity::isIdle()
{
    return e_state == E_STATE_IDLE;
}

bool Entity::isFading()
{
    return e_state == E_STATE_FADING;
}

bool Entity::isGone()
{
    return e_state == E_STATE_GONE;
}

bool Entity::isFocusable()
{
    return focusable && e_state == E_STATE_ACTIVE && p_z > -8000.0f;
}

bool Entity::isAlive()
{
    return life > 0;
}

bool Entity::isPlayer()
{
    return e_obj == OBJ_PLAYER;
}

bool Entity::isCollectable()
{
    return e_obj == OBJ_POWERUP_1;
}

void Entity::activate()
{
    e_state = E_STATE_ACTIVE;
}

void Entity::setType(unsigned short t)
{
    e_type = t;
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

glm::vec3 Entity::getPos()
{
    return glm::vec3(p_x, p_y, p_z);
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

float Entity::getAcceleration()
{
    return acceleration;
}

void Entity::setVelocity(float x, float y, float z)
{
    v_x = x;
    v_y = y;
    v_z = z;
}

void Entity::setVelocityX(float x)
{
    v_x = x;
}

void Entity::setVelocityY(float y)
{
    v_y = y;
}

void Entity::setVelocityZ(float z)
{
    v_z = z;
}

float Entity::getVelocityX()
{
    return v_x;
}

float Entity::getVelocityY()
{
    return v_y;
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
    return (s_x + s_y + s_z) * .333f;
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

void Entity::setRot(float x, float y, float z)
{
    r_x = x;
    r_y = y;
    r_z = z;
}

void Entity::setRotX(float x)
{
    r_x = x;
}

void Entity::setRotY(float y)
{
    r_y = y;
}

void Entity::setRotZ(float z)
{
    r_z = z;
}

float Entity::getRotX()
{
    return r_x;
}

float Entity::getRotY()
{
    return r_y;
}

float Entity::getRotZ()
{
    return r_z;
}

void Entity::setSpin(float x, float y, float z)
{
    w_x = x;
    w_y = y;
    w_z = z;
}

void Entity::setMoney(unsigned short m)
{
    money = m;
}

unsigned short Entity::getMoney()
{
    return money;
}

void Entity::addMoney(unsigned short m)
{
    money += m;
}

int Entity::getLife()
{
    return life;
}

void Entity::setLife(int l)
{
    life = l;
}

float Entity::calcDistanceScale(State &s)
{
    float f = .0001f;

    if (isScenery()) {
        f *= .25f;
    }

    return f * (10000.0f - glm::distance(s.view.getCameraPos(), getPos()));
}

float Entity::calcDistance2D(State &s, shared_ptr<Entity> e)
{
    float x1, y1, x2, y2;

    x1 = E_RELATIVE_MOVEMENT * ((p_x + s.timer_adjustment * v_x) - s.cam_x);
    y1 = E_RELATIVE_MOVEMENT * ((p_y + s.timer_adjustment * v_y) - s.cam_y);

    x2 = E_RELATIVE_MOVEMENT * ((e->getPosX() + s.timer_adjustment * e->getVelocityX()) - s.cam_x);
    y2 = E_RELATIVE_MOVEMENT * ((e->getPosY() + s.timer_adjustment * e->getVelocityY()) - s.cam_y);

    return (1.0f / isqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

float Entity::calcDistance3D(State &s, shared_ptr<Entity> e)
{
    float ds, x1, y1, z1, x2, y2, z2;

    x1 = E_RELATIVE_MOVEMENT * ((p_x + s.timer_adjustment * v_x) - s.cam_x);
    y1 = E_RELATIVE_MOVEMENT * ((p_y + s.timer_adjustment * v_y) - s.cam_y);
    z1 = p_z + s.timer_adjustment * (v_z + E_BASE_SPEED);

    x2 = E_RELATIVE_MOVEMENT * ((e->getPosX() + s.timer_adjustment * e->getVelocityX()) - s.cam_x);
    y2 = E_RELATIVE_MOVEMENT * ((e->getPosY() + s.timer_adjustment * e->getVelocityY()) - s.cam_y);
    z2 = e->getPosZ() + s.timer_adjustment * e->getVelocityZ();

    return (1.0f / isqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2)));
}

bool Entity::isColliding(State &s, shared_ptr<Entity> e)
{
    float r1, r2;

    r1 = getScale()    * (10000.0f + p_z)          * .0002f;
    r2 = e->getScale() * (10000.0f + e->getPosZ()) * .0002f;

    return (calcDistance3D(s, e) < (r1 + r2));
}

void Entity::collide(State &s, shared_ptr<Entity> e)
{
}

void Entity::collect(unsigned short e_obj)
{
}

void Entity::shoot(State &s)
{
}

bool Entity::damage(State &s, int p)
{
    if (isAlive()) {
        life -= p;

        if (life <= 0) {
            e_state = E_STATE_GONE;
        }

        return true;
    }

    return false;
}

void Entity::update(State &s)
{
    float f = 1.0f;

    if (isScenery()) {
        f *= .5f;
    }

    p_x += f * s.timer_adjustment * v_x;
    p_y += f * s.timer_adjustment * v_y;
    p_z += f * s.timer_adjustment * (v_z + E_BASE_SPEED);

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

void Entity::resetTarget()
{
    target = nullptr;
}

void Entity::checkTarget(State &s, shared_ptr<Entity> e)
{
    float ax, ay, hx, hy, dx, dy, dz;

    dx = fabs(e->getPosX() - getPosX());
    dy = fabs(e->getPosY() - getPosY());
    dz = fabs(e->getPosZ());

    hx = 1.0f / isqrt(pow(dz, 2) + pow(dx, 2));
    hy = 1.0f / isqrt(pow(dz, 2) + pow(dy, 2));

    ax = asin(dx / hx) * 180.0f / M_PI;
    ay = asin(dy / hy) * 180.0f / M_PI;

    if (ax < .75f && ay < .75f) {
        if (target) {
            if (calcDistance3D(s, e) < calcDistance3D(s, target)) {
                target = e;
            }
        } else {
            target = e;
        }
    }
}

bool Entity::hasTarget(shared_ptr<Entity> e)
{
    return e == target;
}

void Entity::drawCrosshair(State &s, shared_ptr<Entity> me)
{
    float a = float(s.global_alpha) * .01f;
    float scale = 1.25f * (150.0f + ((p_z + 12500.0f) * .00005f));
    float rot, da;

    // let cross-hair fade out when player dies
    if (isAlive()) {
        da = .85f;
    } else {
        if (da > .01f) {
            da -= s.timer_adjustment * .01f;
        } else {
            da = .0f;
        }
    }

    // let cross-hair fade out when too near
    if (p_z > -1000.0f) {
        da -= .001f * (1000.0f + p_z);
    }

    glDisable(GL_DEPTH_TEST);

    s.shaders[S_TEXTURE]->bind();

    if (s.player->hasTarget(me)) {
        // target locked, auto-aiming active

        s.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(t_r, t_g, t_b, a * da));
        s.shaders[S_TEXTURE]->update(UNI_MVP, s.view.transform(
            (p_x - s.cam_x) * E_RELATIVE_MOVEMENT,
            (p_y - s.cam_y) * E_RELATIVE_MOVEMENT,
            p_z - .1f,

            s.tilt_y * -.035f,
            s.tilt_x * -.035f,
            0,

            scale,
            scale,
            0
        ));

        s.textures[T_HUD_3]->bind();
        s.textures[T_HUD_3]->draw();
    } else {
        // aiming help

        rot = (-180.0f / M_PI) * atan(
            (s.player->getPosY() - getPosY()) /
            (s.player->getPosX() - getPosX())
        );

        if (s.player->getPosX() > getPosX()) {
            rot += 180.0f;
        }

        s.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, a * da));
        s.shaders[S_TEXTURE]->update(UNI_MVP, s.view.transform(
            (p_x - s.cam_x) * E_RELATIVE_MOVEMENT,
            (p_y - s.cam_y) * E_RELATIVE_MOVEMENT,
            p_z - .1f,

            s.tilt_y * -.035f,
            s.tilt_x * -.035f,
            rot,

            scale,
            scale,
            0
        ));

        s.textures[T_HUD_4]->bind();
        s.textures[T_HUD_4]->draw();
    }

    s.shaders[S_TEXTURE]->update(UNI_MVP, s.view.transform(
        (p_x - s.cam_x) * E_RELATIVE_MOVEMENT,
        (p_y - s.cam_y) * E_RELATIVE_MOVEMENT,
        p_z,

        s.tilt_y * -.035f,
        s.tilt_x * -.035f,
        0,

        scale * 1.5f,
        scale * 1.5f,
        0
    ));

    glEnable(GL_DEPTH_TEST);

    s.textures[T_HUD_3]->bind();
    s.textures[T_HUD_3]->draw();

    s.shaders[S_TEXTURE]->unbind();
}

void Entity::draw(State &s)
{
    float a = float(s.global_alpha) * .01f, d = calcDistanceScale(s);

    glm::vec4 color = glm::vec4(c_r * a, c_g * a, c_b * a, a);

    glm::mat4 projection = s.view.getPerspective();
    glm::mat4 view = s.view.getCamera();
    glm::mat4 model = s.view.getModel(
        (getPosX() - s.cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - s.cam_y) * E_RELATIVE_MOVEMENT,
        getPosZ(),

        getRotX(),
        getRotY(),
        getRotZ(),

        getScaleX() * d,
        getScaleY() * d,
        getScaleZ() * d
    );

    s.models[e_obj]->draw(model, view, projection, color);
}
