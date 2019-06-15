#include "asteroid.hpp"

Asteroid::Asteroid() : Entity()
{
    e_id = OBJ_ASTEROID_1;
    e_type = OBJ_TYPE_COLLIDER;
    e_state = OBJ_STATE_IDLE;
}

Asteroid::~Asteroid()
{
}

bool Asteroid::damage(State &s, int p)
{
    bool destroyed = Entity::damage(s, p);

    if (destroyed) {
        s.audio.playSample(SFX_EXPLOSION_2, 192, 180);
        s.entities.push_back(make_shared<Explosion>(OBJ_EXPLOSION_2, p_x, p_y, p_z));
        s.entities.push_back(make_shared<Explosion>(OBJ_EXPLOSION_3, p_x, p_y, p_z - 15.0f));
    }

    return destroyed;
}

void Asteroid::move(State &s)
{
    p_x += s.timer_adjustment * v_x;
    p_y += s.timer_adjustment * v_y;

    if (e_type == OBJ_TYPE_SCENERY) {
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

    if (p_z > .0f) {
        e_state = OBJ_STATE_GONE;
    }
}

void Asteroid::draw(State &s)
{
    float scale, m, a = float(s.global_alpha) * .01f;

    if (e_type == OBJ_TYPE_SCENERY) {
        m = .85f;
        scale = (10000.0f + p_z) * .0000333f;
    } else {
        m = 1.0f;
        scale = (10000.0f + p_z) * .0001f;
    }

    glLoadIdentity();

    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // light setup
    GLfloat col_specular[] = { a, a, a, a };
    glLightfv(GL_LIGHT0, GL_SPECULAR, col_specular);

    GLfloat col_ambient[] = { .25f, .225f, .2f, a };
    glLightfv(GL_LIGHT0, GL_AMBIENT, col_ambient);

    GLfloat col_diffuse[] = { .0f, .0f, .0f, a };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, col_diffuse);

    GLfloat pos_ambient[] = { .5f, .5f, .0f, a };
    glLightfv(GL_LIGHT0, GL_POSITION, pos_ambient);

    glEnable(GL_LIGHT0);

    // material setup
    GLfloat col_emission[] = { .25f * a, .225f * a, .2f * a, a };

    glMaterialfv(GL_FRONT, GL_AMBIENT, col_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, col_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, col_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, col_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, 4.0f);

    glPushMatrix();

    glRotatef(s.tilt_x * -.035f,  .0f, 1.0f, .0f);
    glRotatef(s.tilt_y * -.035f, 1.0f,  .0f, .0f);

    glTranslatef(
        E_RELATIVE_MOVEMENT * m * (p_x - s.cam_x),
        E_RELATIVE_MOVEMENT * m * (p_y - s.cam_y),
        p_z
    );

    glRotatef(r_x, 1.0f,  .0f,  .0f);
    glRotatef(r_y,  .0f, 1.0f,  .0f);
    glRotatef(r_z,  .0f,  .0f, 1.0f);

    glScalef(s_x * scale, s_y * scale, s_z * scale);

    glCallList(*s.models[e_id]);

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_NORMALIZE);
    glDisable(GL_CULL_FACE);

    glPopMatrix();
}
