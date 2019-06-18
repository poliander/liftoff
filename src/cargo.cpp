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
        s.entities.push_back(make_shared<Explosion>(OBJ_EXPLOSION_5, p_x, p_y, p_z));
        s.entities.push_back(make_shared<Powerup>(p_x, p_y, p_z));

        yield = true;
    }

    return damaged;
}

void Cargo::move(State &s)
{
    Entity::move(s);

    counter += s.timer_adjustment * 0.1f;

    if (p_z > 100.0f) {
        e_state = E_STATE_GONE;
    }
}

void Cargo::draw(State &s)
{
    float a = float(s.global_alpha) * .01f;

    float scale1 = (10000.0f + p_z) * .0001f;
    float scale2 = (1.0f + sin(counter * (M_PI / 180.0f))) * .5f;

    if (p_z < -8000.0f) {
        a = (p_z + 10000.0f) * .0005f;
    }

    glLoadIdentity();
    glRotatef(s.tilt_x * -.035f, 0, 1, 0);
    glRotatef(s.tilt_y * -.035f, 1, 0, 0);

    // light setup
    glEnable(GL_LIGHTING);
    GLfloat col_ambient[] = { a, a, a };
    GLfloat pos_ambient[] = {.5f, .5f, .0f, .0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, col_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, pos_ambient);
    glEnable(GL_LIGHT0);

    // material setup
    GLfloat col_specular[] = { a, a, a };
    GLfloat col_emission[] = { (.2f + scale2) * a, (.4f + scale2) * a, (.8f + scale2) * a };

    glMaterialfv(GL_FRONT, GL_EMISSION, col_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, col_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, col_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);

    glTranslatef(
        (p_x - s.cam_x) * E_RELATIVE_MOVEMENT,
        (p_y - s.cam_y) * E_RELATIVE_MOVEMENT,
        (p_z)
    );

    glPushMatrix();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glRotatef(r_x, 1, 0, 0);
    glRotatef(r_y, 0, 1, 0);
    glRotatef(r_z, 0, 0, 1);

    glScalef(s_x * scale1, s_y * scale1, s_z * scale1);

    glCallList(*s.models[e_obj]);

    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindTexture(GL_TEXTURE_2D, *s.textures[T_GLOW]);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    scale2 += .5f;
    glColor4f(.5f, .75f, 1.0f, (.15f + .1f * scale2) * s.global_alpha * .01f);
    scale2 *= 7.5f;
    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(-scale2, -scale2, 0);

      glTexCoord2f(0, 1);
      glVertex3f(-scale2, scale2, 0);

      glTexCoord2f(1, 1);
      glVertex3f(scale2, scale2, 0);

      glTexCoord2f(1, 0);
      glVertex3f(scale2, -scale2, 0);
    glEnd();

    glRotatef(90.0f, 1, 0, 0);
    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(-scale2, -scale2, 0);

      glTexCoord2f(0, 1);
      glVertex3f(-scale2, scale2, 0);

      glTexCoord2f(1, 1);
      glVertex3f(scale2, scale2, 0);

      glTexCoord2f(1, 0);
      glVertex3f(scale2, -scale2, 0);
    glEnd();

    glRotatef(90.0f, 0, 1, 0);
    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(-scale2, -scale2, 0);

      glTexCoord2f(0, 1);
      glVertex3f(-scale2, scale2, 0);

      glTexCoord2f(1, 1);
      glVertex3f(scale2, scale2, 0);

      glTexCoord2f(1, 0);
      glVertex3f(scale2, -scale2, 0);
    glEnd();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPopMatrix();
}
