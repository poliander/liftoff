#include "explosion.hpp"

Explosion::Explosion(State& s, uint16_t t, float x, float y, float z) : Entity(s) {
    e_obj = t;
    e_type = E_TYPE_DECORATION;
    e_state = E_STATE_ACTIVE;

    setPos(x, y, z);
    setRot(
        static_cast<float>(rand() % 360),
        static_cast<float>(rand() % 360),
        static_cast<float>(rand() % 360)
    );

    particles = make_unique<ParticleEngine>(s);

    switch (e_obj) {
        // green laser gun impact
        case OBJ_EXPLOSION_1:
            particles->setup(EMIT_EXPLOSION, 20, .25f, .25f, .25f, .1f, 20.0f);
            particles->setTexture(T_EXPLOSION_1);
            particles->setColor(.5f, 1.0f, .8f);
            particles->setIncrease(1.05f);
            break;

        // explosion smoke
        case OBJ_EXPLOSION_2:
            particles->setup(EMIT_EXPLOSION, 30, .25f, .25f, .25f, .025f, 100.0f);
            particles->setTexture(T_EXPLOSION_2);
            particles->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            particles->setColor(0.75f, 0.75f, 0.75f);
            particles->setAlpha(0.333f);
            particles->setInflation(-.01f);
            break;

        // explosion sparks
        case OBJ_EXPLOSION_3:
            particles->setup(EMIT_EXPLOSION, 30, 30.0f, 30.0f, 30.0f, .02f, 30.0f);
            particles->setTexture(T_STAR);
            particles->setColor(1.0f, 1.0f, 0.8f);
            particles->setInflation(-.1f);
            particles->setIncrease(-0.75f);
            break;

        // explosion fireball
        case OBJ_EXPLOSION_4:
            particles->setup(EMIT_EXPLOSION, 20, .1f, .1f, .1f, .05f, 50.0f);
            particles->setTexture(T_EXPLOSION_1);
            particles->setColor(1.0f, 0.8f, 0.6f);
            particles->setAlpha(0.5f);
            particles->setIncrease(1.05f);
            particles->setInflation(-.01f);
            break;
    }
}

Explosion::Explosion(State& s, uint16_t t, float x, float y, float z, float r, float g, float b) : Explosion(s, t, x, y, z) {
    particles->setColor(r, g, b);
}

Explosion::~Explosion() {
}

void Explosion::update() {
    Entity::update();

    particles->update();

    if (getPosZ() > 0 || particles->done()) {
        e_state = E_STATE_GONE;
    }
}

void Explosion::draw() {
    particles->draw(
        (getPosX() - state.cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - state.cam_y) * E_RELATIVE_MOVEMENT,
        getPosZ(),

        getRotX(),
        getRotY(),
        getRotZ()
    );
}
