#include "cargo.hpp"

Cargo::Cargo() : Entity()
{
    e_obj = OBJ_CARGO_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_IDLE;

    focusable = true;
    money = 50;
}

Cargo::~Cargo()
{
}

bool Cargo::damage(State &s, int p)
{
    bool damaged = Entity::damage(s, p);

    if (damaged && e_state == E_STATE_GONE && yield == false) {
        s.audio.playSample(SFX_EXPLOSION_1, 192, 180);

        s.addMessage(money, MSG_MONEY);
        s.player->addMoney(money);

        s.entities.push_back(make_shared<Explosion>(OBJ_EXPLOSION_3, p_x, p_y, p_z));
        s.entities.push_back(make_shared<Powerup>(p_x, p_y, p_z));

        yield = true;
    }

    return damaged;
}

void Cargo::update(State &s)
{
    Entity::update(s);

    counter += s.timer_adjustment * 0.1f;

    if (p_z > 0) {
        e_state = E_STATE_GONE;
    }
}
