#include "powerup.hpp"

Powerup::Powerup(float x, float y, float z) : Entity()
{
    e_obj = OBJ_POWERUP_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_ACTIVE;

    focusable = true;
    counter = 0;

    particles = new ParticleEngine();
    particles->setup(EMITTER_EXPLOSION, 15, .5f, .5f, .5f, .5f, 1.0f);
    particles->setColor(.6f, .75f, 1.0f);
    particles->setAlpha(.5f);
    particles->setScale(10.0f);
    particles->setContinuous(true);

    setPos(x, y, z);
    setScale(50.0f, 50.0f, 50.0f);

    t_r = 0.3f;
    t_g = 0.55f;
    t_b = 1.0f;
}

Powerup::~Powerup()
{
    delete particles;
}

bool Powerup::damage(State &s, int p)
{
    return false;
}

void Powerup::collide(State &s, shared_ptr<Entity> e)
{
    if (e_state == E_STATE_ACTIVE && e->isPlayer()) {
        s.audio.playSample(SFX_POWERUP_1, 192, 180);
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
        counter += s.timer_adjustment * .1f;
    }

    if (getPosZ() > 0 || counter > 1.5f) {
        e_state = E_STATE_GONE;
    }
}

void Powerup::draw(State &s)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glRotatef(s.tilt_x * -.035f, 0, 1, 0);
    glRotatef(s.tilt_y * -.035f, 1, 0, 0);

    s.textures[T_STAR]->bind();

    particles->setSize (150.0f / (1.0f + counter * 75.0f));
    particles->setAlpha(1.0f - (counter * (1.0f / 1.5f)));
    particles->setScale(10.0f + (7.5f * sin(counter * M_PI)));
    particles->draw(s,
        (getPosX() - s.cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - s.cam_y) * E_RELATIVE_MOVEMENT,
        (getPosZ()),
        (getRotX()),
        (getRotY()),
        (getRotZ())
    );

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

