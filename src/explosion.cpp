#include "explosion.hpp"

Explosion::Explosion(short int type, float x, float y, float z) : Entity()
{
    e_obj = type;
    e_type = E_TYPE_DECORATION;
    e_state = E_STATE_ACTIVE;

    setPos(x, y, z);
    setRot(float(rand() % 360), float(rand() % 360), float(rand() % 360));

    particles = make_unique<ParticleEngine>();

    switch (type) {
        // green laser gun impact
        case OBJ_EXPLOSION_1:
            particles->setup(EMIT_EXPLOSION, 20, .25f, .25f, .25f, .1f, 20.0f);
            particles->setColor(.5f, 1.0f, .8f);
            particles->setIncrease(1.05f);
            break;

        // explosion smoke
        case OBJ_EXPLOSION_2:
            particles->setup(EMIT_EXPLOSION, 30, .25f, .25f, .25f, .025f, 100.0f);
            particles->setColor(0.75f, 0.75f, 0.75f);
            particles->setAlpha(0.333f);
            particles->setInflation(-.01f);
            break;

        // explosion sparks
        case OBJ_EXPLOSION_3:
            particles->setup(EMIT_EXPLOSION, 30, 50.0f, 50.0f, 50.0f, .04f, 40.0f);
            particles->setColor(1.0f, 1.0f, 0.8f);
            particles->setInflation(-.15f);
            particles->setIncrease(-1.0f);
            break;

        // explosion fireball
        case OBJ_EXPLOSION_4:
            particles->setup(EMIT_EXPLOSION, 20, .01f, .01f, .01f, .05f, 50.0f);
            particles->setColor(1.0f, 0.8f, 0.6f);
            particles->setAlpha(0.5f);
            particles->setIncrease(1.05f);
            break;
    }
}

Explosion::Explosion(short int type, float x, float y, float z, float r, float g, float b) : Explosion(type, x, y, z)
{
    particles->setColor(r, g, b);
}

Explosion::~Explosion()
{
}

void Explosion::update(State &s)
{
    Entity::update(s);

    particles->update(s);

    if (getPosZ() > 0 || particles->isGone()) {
        e_state = E_STATE_GONE;
    }
}

void Explosion::draw(State &s)
{
    switch (e_obj) {

        // green laser gun impact
        case OBJ_EXPLOSION_1:
            s.textures[T_EXPLOSION_1]->bind();
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        // explosion smoke
        case OBJ_EXPLOSION_2:
            s.textures[T_EXPLOSION_2]->bind();
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;

        // explosion sparks
        case OBJ_EXPLOSION_3:
            s.textures[T_STAR]->bind();
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            if (particles->getInflation() > .1f) {
                particles->setInflation(particles->getInflation() * .9f * s.timer_adjustment);
            }

            break;

        // explosion fireball
        case OBJ_EXPLOSION_4:
            s.textures[T_EXPLOSION_1]->bind();
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
    }

    particles->draw(s,
        (getPosX() - s.cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - s.cam_y) * E_RELATIVE_MOVEMENT,
        getPosZ(),

        getRotX(),
        getRotY(),
        getRotZ()
    );

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
