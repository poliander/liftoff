#include "missile.hpp"

Missile::Missile() : Entity()
{
    e_obj = OBJ_MISSILE_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_ACTIVE;

    c_r = 0.5f;
    c_g = 1.0f;
    c_b = 0.8f;
    c_a = 1.0f;

    v_x = 0;
    v_y = 0;
    v_z = -125.0f;

    s_x = 2.25f;
    s_y = 2.25f;
    s_z = 2.25f;

    power = 20;
}

Missile::~Missile()
{
}

void Missile::collide(State &s, shared_ptr<Entity> e)
{
    if (e->damage(s, power)) {
        s.audio.playSample(SFX_GUN_IMPACT, 192, 180);
        s.entities.push_back(make_shared<Explosion>(OBJ_EXPLOSION_1, p_x, p_y, p_z));

        e_state = E_STATE_GONE;
    }
}

void Missile::move(State &s)
{
    Entity::move(s);

    c_a = (s.global_alpha * .005f) + ((p_z + 200.0f) * .00002f);

    if (c_a < 0 || p_z < -10000.0f) {
        e_state = E_STATE_GONE;
    }
}

void Missile::draw(State &s)
{
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, *s.textures[T_MISSILE_1]);
    glColor4f(c_r, c_g, c_b, c_a);

    glPushMatrix();

    glTranslatef(
        E_RELATIVE_MOVEMENT * (p_x - s.cam_x),
        E_RELATIVE_MOVEMENT * (p_y - s.cam_y),
        p_z
    );

    glRotatef(270, 0, 0, 1);
    glScalef(s_x, s_y, s_z);

    glBegin (GL_QUADS);
      glTexCoord2f (0, 0);
      glVertex3f (-3.0f, 0, 0);

      glTexCoord2f (1, 0);
      glVertex3f (3.0f, 0, 0);

      glTexCoord2f (1, .95f);
      glVertex3f (3.0f, 0, 150.0f);

      glTexCoord2f (0, .95f);
      glVertex3f (-3.0f, 0, 150.0f);
    glEnd();

    glRotatef(90, 0, 0, 1);

    glBegin (GL_QUADS);
      glTexCoord2f (0, 0);
      glVertex3f (-3.0f, 0, 0);

      glTexCoord2f (1, 0);
      glVertex3f (3.0f, 0, 0);

      glTexCoord2f (1, .95f);
      glVertex3f (3.0f, 0, 150.0f);

      glTexCoord2f (0, .95f);
      glVertex3f (-3.0f, 0, 150.0f);
    glEnd();

    glPopMatrix();
}
