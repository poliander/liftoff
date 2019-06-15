#include "powerup.hpp"

Powerup::Powerup() : Entity()
{
    particles = new ParticleEngine();
    particles->setup(EMITTER_EXPLOSION, 20, .4f, .4f, .4f, 25.0f, 5.0f);
}

Powerup::~Powerup()
{
    delete particles;
}

void Powerup::move(State &s)
{
    particles->move(s);

    if (e_state == OBJ_STATE_FADING) {
        counter += s.timer_adjustment * .1f;
    }

    p_z += s.timer_adjustment * E_BASE_SPEED;

    if (p_z > 100.0f || counter > 1.0f) {
        e_state = OBJ_STATE_GONE;
    }
}

void Powerup::draw(State &s)
{
    float a = float(s.global_alpha) * .01f;

    glLoadIdentity();
    glPushMatrix();

    glRotatef(s.tilt_x * -.035f, 0, 1, 0);
    glRotatef(s.tilt_y * -.035f, 1, 0, 0);

    glTranslatef(
        (p_x - s.cam_x) * E_RELATIVE_MOVEMENT,
        (p_y - s.cam_y) * E_RELATIVE_MOVEMENT,
        (p_z)
    );

    glShadeModel(GL_FLAT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBindTexture(GL_TEXTURE_2D, *s.textures[T_EXPLOSION_3]);

    particles->setAlpha(a * (1.0f - counter));
    particles->setSize(3.0f / (1.0f + counter * 5.0f));
    particles->setColor(.6f, .75f, 1.0f);
    particles->setScale(25.0f + counter * 100.0f);
    particles->draw(s, 0, 0, 0, r_x, r_y, r_z);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    glPopMatrix();
}

