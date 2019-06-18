#include "debris.hpp"

Debris::Debris() : Entity()
{
    e_id = OBJ_DEBRIS_1;
    e_type = OBJ_TYPE_SCENERY;
    e_state = OBJ_STATE_IDLE;

    p_x = -600.0f + float(rand() % 1200);
    p_y = -400.0f + float(rand() % 800);
    p_z = -8000.0f;

    w_x = float(rand() % 100) * .1f;
    w_y = float(rand() % 100) * .1f;
    w_z = float(rand() % 100) * .1f;

    s_x = 20.0f + float(rand() % 400) * .2f;
    s_y = 20.0f + float(rand() % 400) * .2f;
    s_z = 20.0f + float(rand() % 400) * .2f;
}

Debris::~Debris()
{
}

void Debris::move(State &s)
{
    Entity::move(s);

    if (p_z > 0) {
        e_state = OBJ_STATE_GONE;
    }
}

void Debris::draw(State &s)
{
    float a = float(s.global_alpha) * .01f;
    float scale = (10000.0f + p_z) * .0001f;

    glLoadIdentity();

    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();

    glTranslatef(
        E_RELATIVE_MOVEMENT * (p_x - s.cam_x),
        E_RELATIVE_MOVEMENT * (p_y - s.cam_y),
        p_z
    );

    glRotatef(r_x, 1.0f,  .0f,  .0f);
    glRotatef(r_y,  .0f, 1.0f,  .0f);
    glRotatef(r_z,  .0f,  .0f, 1.0f);

    glScalef(s_x * scale, s_y * scale, s_z * scale);

    glColor4f(c_r, c_g, c_b, c_a * a);
    glCallList(*s.models[e_id]);

    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_NORMALIZE);
    glDisable(GL_CULL_FACE);
}
