#include "powerup.hpp"

Powerup::Powerup(State &s) : Object(s)
{
    particles = new ParticleEngine();
    particles->setup(EMITTER_EXPLOSION, 20, .4f, .4f, .4f, 25.0f, 5.0f);
}

Powerup::~Powerup()
{
    delete particles;
}

void Powerup::draw(int oid)
{
    float a;

    glLoadIdentity();
    glPushMatrix();

    glRotatef(state.tilt_x * -.035f, 0, 1, 0);
    glRotatef(state.tilt_y * -.035f, 1, 0, 0);

    glTranslatef(
        (state.objects[oid].pos_x - state.cam_x) * E_RELATIVE_MOVEMENT,
        (state.objects[oid].pos_y - state.cam_y) * E_RELATIVE_MOVEMENT,
        (state.objects[oid].pos_z)
    );

    glShadeModel(GL_FLAT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBindTexture(GL_TEXTURE_2D, state.texture[T_EXPLOSION_3]);

    a = 1.5f - state.objects[oid].cnt * 1.5f;

    particles->setAlpha(a * float(state.global_alpha) * .01f);
    particles->setSize(3.0f / (1.0f + state.objects[oid].cnt * 5.0f));
    particles->setColor(.6f, .75f, 1.0f);
    particles->setScale(25.0f + state.objects[oid].cnt * 100.0f);
    particles->draw(
        state,
        .0f,
        .0f,
        .0f,
        state.objects[oid].rot_x,
        state.objects[oid].rot_y,
        state.objects[oid].rot_z
    );

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    glPopMatrix();
}

void Powerup::move(int oid)
{
    particles->move(state);

    if (state.objects[oid].state == OBJ_STATE_FADING) {
        state.objects[oid].cnt += state.timer_adjustment * .05f;

        if (state.objects[oid].cnt > 1.0f) {
            state.remove(oid);
        }

        state.objects[oid].pos_z += state.timer_adjustment * state.objects[state.player].speed * .25f;
    } else {
        state.objects[oid].pos_z += state.timer_adjustment * state.objects[state.player].speed;
    }
}
