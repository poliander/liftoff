#include "debris.hpp"

Debris::Debris(State &s) : Entity(s)
{
    e_obj = OBJ_DEBRIS_1;
    e_type = E_TYPE_DECORATION;
    e_state = E_STATE_IDLE;

    p_x = -600.0f + static_cast<float>(rand() % 1200);
    p_y = -400.0f + static_cast<float>(rand() % 800);
    p_z = -8000.0f;

    w_x = static_cast<float>(rand() % 100) * .1f;
    w_y = static_cast<float>(rand() % 100) * .1f;
    w_z = static_cast<float>(rand() % 100) * .1f;

    s_x = 20.0f + static_cast<float>(rand() % 400) * .2f;
    s_y = 20.0f + static_cast<float>(rand() % 400) * .2f;
    s_z = 20.0f + static_cast<float>(rand() % 400) * .2f;

    c_r = .6f;
    c_g = .6f;
    c_b = .6f;
}

Debris::~Debris()
{
}

void Debris::update()
{
    Entity::update();

    if (p_z > 0) {
        e_state = E_STATE_GONE;
    }
}
