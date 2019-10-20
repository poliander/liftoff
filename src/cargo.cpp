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
        s.entities.push_back(make_shared<Powerup>(p_x, p_y, p_z));

        yield = true;
    }

    return damaged;
}

void Cargo::update(State &s)
{
    Entity::update(s);

    counter += s.timer_adjustment * 0.1f;

    if (p_z > 0) {
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

    s.models[e_obj]->draw(s.view.transform(
        (getPosX() - s.cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - s.cam_y) * E_RELATIVE_MOVEMENT,
        (getPosZ()),

        getRotX() + s.tilt_y * -.035f,
        getRotY() + s.tilt_x * -.035f,
        getRotZ(),

        getScaleX() * scale1,
        getScaleY() * scale1,
        getScaleZ() * scale1
    ), glm::vec4(c_r, c_g, c_b, c_a));

    glLoadIdentity();
    glPushMatrix();

    s.textures[T_GLOW]->bind();

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

    glPopMatrix();
}
