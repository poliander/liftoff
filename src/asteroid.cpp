#include "asteroid.hpp"

Asteroid::Asteroid() : Entity()
{
    e_obj = OBJ_ASTEROID_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_IDLE;
}

Asteroid::~Asteroid()
{
}

bool Asteroid::damage(State &s, int p)
{
    bool damaged = Entity::damage(s, p);

    if (damaged && !isAlive()) {
        unsigned short m = int(getScale() * .5f);

        s.audio.playSample(SFX_EXPLOSION_2, 192, 180);

        s.entities.push_back(make_shared<Explosion>(OBJ_EXPLOSION_2, p_x, p_y, p_z));
        s.entities.push_back(make_shared<Explosion>(OBJ_EXPLOSION_3, p_x, p_y, p_z + 5.0f));
        s.entities.push_back(make_shared<Explosion>(OBJ_EXPLOSION_4, p_x, p_y, p_z + 10.0f));

        s.addMessage(m, MSG_MONEY);
        s.player->addMoney(m);
    }

    return damaged;
}

void Asteroid::move(State &s)
{
    p_x += s.timer_adjustment * v_x;
    p_y += s.timer_adjustment * v_y;

    if (e_type == E_TYPE_SCENERY) {
        p_z += s.timer_adjustment * (v_z + E_BASE_SPEED) * .5f;
    } else {
        p_z += s.timer_adjustment * (v_z + E_BASE_SPEED);
    }

    r_x += s.timer_adjustment * w_x * .1f;
    if (r_x < 0) r_x += 360.0f;
    if (r_x > 360.0f) r_x -= 360.0f;

    r_y += s.timer_adjustment * w_y * .1f;
    if (r_y < 0) r_y += 360.0f;
    if (r_y > 360.0f) r_y -= 360.0f;

    r_z += s.timer_adjustment * w_z * .1f;
    if (r_z < 0) r_z += 360.0f;
    if (r_z > 360.0f) r_z -= 360.0f;

    if (p_z > 0) {
        e_state = E_STATE_GONE;
    }
}

void Asteroid::draw(State &s)
{
    float scale, m, a = float(s.global_alpha) * .01f;

    if (e_type == E_TYPE_SCENERY) {
        m = .85f;
        scale = (10000.0f + p_z) * .0000333f;
    } else {
        m = 1.0f;
        scale = (10000.0f + p_z) * .0001f;
    }

    s.models[e_obj]->draw(s.view.transform(
        E_RELATIVE_MOVEMENT * m * (p_x - s.cam_x),
        E_RELATIVE_MOVEMENT * m * (p_y - s.cam_y),
        p_z,

        r_x + s.tilt_y * -.035f,
        r_y + s.tilt_x * -.035f,
        r_z,

        s_x * scale,
        s_y * scale,
        s_z * scale
    ), glm::vec4(c_r, c_g, c_b, c_a));
}
