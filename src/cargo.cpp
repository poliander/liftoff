#include "cargo.hpp"

Cargo::Cargo(State &s) : Object(s)
{
    memset(&model, 0, sizeof(struct obj_model_t));
    shininess = 64.0f;
}

Cargo::~Cargo()
{
}

void Cargo::draw(int oid)
{
    float a = 1.0f;
    float s = 1.0f, s2;

    if (state.objects[oid].pos_z < -8000.0f) {
        a = (state.objects[oid].pos_z + 10000.0f) * .0005f;
    }

    a *= float(state.global_alpha) * .01f;
    s = (10000.0f + state.objects[oid].pos_z) * .0001f;
    s2 = (1.0f + sin(state.objects[oid].cnt * (M_PI / 180.0f))) * .5f;

    glLoadIdentity();
    glRotatef(state.tilt_x * -.035f, 0, 1, 0);
    glRotatef(state.tilt_y * -.035f, 1, 0, 0);

    // light setup
    glEnable(GL_LIGHTING);
    GLfloat col_ambient[] = { a, a, a };
    GLfloat pos_ambient[] = {.5f, .5f, .0f, .0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, col_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, pos_ambient);
    glEnable(GL_LIGHT0);

    // material setup
    GLfloat col_specular[] = { a, a, a };
    GLfloat col_emission[] = { (.2f+s2)*a, (.4f+s2)*a, (.8f+s2)*a };

    glMaterialfv(GL_FRONT, GL_EMISSION, col_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, col_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, col_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glTranslatef(
        (state.objects[oid].pos_x - state.cam_x) * E_RELATIVE_MOVEMENT,
        (state.objects[oid].pos_y - state.cam_y) * E_RELATIVE_MOVEMENT,
        (state.objects[oid].pos_z)
    );

    glPushMatrix();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glRotatef(state.objects[oid].rot_x, 1, 0, 0);
    glRotatef(state.objects[oid].rot_y, 0, 1, 0);
    glRotatef(state.objects[oid].rot_z, 0, 0, 1);

    glScalef(
        state.objects[oid].scale_x * s,
        state.objects[oid].scale_y * s,
        state.objects[oid].scale_z * s
    );

    glCallList(model_list);

    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindTexture(GL_TEXTURE_2D, textures[1]);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    s2 += .5f;
    glColor4f(.5f, .75f, 1.0f, (.15f + .1f * s2) * state.global_alpha * .01f);
    s2 *= 7.5f;
    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(-s2, -s2, 0);

      glTexCoord2f(0, 1);
      glVertex3f(-s2, s2, 0);

      glTexCoord2f(1, 1);
      glVertex3f(s2, s2, 0);

      glTexCoord2f(1, 0);
      glVertex3f(s2, -s2, 0);
    glEnd();

    glRotatef(90.0f, 1, 0, 0);
    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(-s2, -s2, 0);

      glTexCoord2f(0, 1);
      glVertex3f(-s2, s2, 0);

      glTexCoord2f(1, 1);
      glVertex3f(s2, s2, 0);

      glTexCoord2f(1, 0);
      glVertex3f(s2, -s2, 0);
    glEnd();

    glRotatef(90.0f, 0, 1, 0);
    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(-s2, -s2, 0);

      glTexCoord2f(0, 1);
      glVertex3f(-s2, s2, 0);

      glTexCoord2f(1, 1);
      glVertex3f(s2, s2, 0);

      glTexCoord2f(1, 0);
      glVertex3f(s2, -s2, 0);
    glEnd();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPopMatrix();

    drawCrosshair(oid, 1.0f, .55f, .3f);
}
