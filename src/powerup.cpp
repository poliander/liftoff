#include "powerup.hpp"

Powerup::Powerup(float x, float y, float z) : Entity()
{
    e_obj = OBJ_POWERUP_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_ACTIVE;

    focusable = true;
    counter = 0;
    life = 1;

    particles = make_unique<ParticleEngine>();
    particles->setup(EMIT_EXPLOSION, 15, .5f, .5f, .5f, .05f, 200.0f);
    particles->setTexture(T_STAR);
    particles->setColor(.6f, .75f, 1.0f);
    particles->setAlpha(.5f);
    particles->setVolume(10.0f);
    particles->setContinuous(true);

    setPos(x, y, z);
    setScale(50.0f, 50.0f, 50.0f);

    t_r = 0.3f;
    t_g = 0.55f;
    t_b = 1.0f;
}

Powerup::~Powerup()
{
}

bool Powerup::damage(State &s, int p)
{
    return false;
}

void Powerup::collide(State &s, shared_ptr<Entity> e)
{
    if (e_state == E_STATE_ACTIVE && e->isPlayer()) {
        s.audio.playSample(SFX_POWERUP_1, 192, 180);
        s.notify(MSG_ENERGY, 500);
        e->collect(e_obj);
        e_state = E_STATE_FADING;
        v_z = -.5f * E_BASE_SPEED;
    }
}

void Powerup::update(State &s)
{
    Entity::update(s);

    particles->update(s);

    if (e_state == E_STATE_FADING) {
        counter += s.timer_adjustment * .01f;
        particles->setContinuous(false);
        particles->setIncrease(-10.0f);
    }

    if (getPosZ() > 0 || counter > 5.0f) {
        e_state = E_STATE_GONE;
    }
}

void Powerup::draw(State &s)
{
    particles->draw(s,
        (getPosX() - s.cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - s.cam_y) * E_RELATIVE_MOVEMENT,
        (getPosZ()),
        (getRotX()),
        (getRotY()),
        (getRotZ())
    );
}
