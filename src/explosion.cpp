#include "explosion.hpp"

Explosion::Explosion(unsigned short int type, float x, float y, float z) : Entity()
{
    e_id = type;
    e_type = OBJ_TYPE_SCENERY;
    e_state = OBJ_STATE_ACTIVE;

    particles = new ParticleEngine();
    particles->setup(EMITTER_EXPLOSION, 25, .15f, .15f, .1f, 3.0f, 1.25f);

    p_x = x;
    p_y = y;
    p_z = z;

    r_x = float(rand() % 360);
    r_y = float(rand() % 360);
    r_z = float(rand() % 360);

    switch (type) {
        // green laser gun impact
        case OBJ_EXPLOSION_1:
            timer = 500;
            break;

        // explosion smoke
        case OBJ_EXPLOSION_2:
            timer = 4000;
            break;

        // explosion fireball
        case OBJ_EXPLOSION_3:
            timer = 2000;
            break;

        // collision sparks
        case OBJ_EXPLOSION_4:
            timer = 1000;
            break;

        // explosion nova
        case OBJ_EXPLOSION_5:
            timer = 1500;
            break;
    }
}

Explosion::~Explosion()
{
    delete particles;
}

void Explosion::move(State &s)
{
    particles->move(s);

    timer -= s.timer_adjustment * 22.5f;
    counter += (7.0f - counter * 1.5f) * s.timer_adjustment * .1f;

    Entity::move(s);

    if (timer < 0 || p_z > 100.0f) {
        e_state = OBJ_STATE_GONE;
    }
}

void Explosion::draw(State &s)
{
    bool use_particles = true;

    glLoadIdentity();

    switch (e_id) {

        // green laser gun impact
        case OBJ_EXPLOSION_1:
            particles->setAlpha(1.4f - counter * .002f);
            particles->setColor(.5f, 1.0f, .8f);
            particles->setSize(2.5f + counter * .005f);
            particles->setScale(2.75f + counter * .005f);

            glBindTexture(GL_TEXTURE_2D, s.texture[T_EXPLOSION_1]);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        // explosion smoke
        case OBJ_EXPLOSION_2:
            particles->setAlpha(.5f - counter * .0001165f);
            particles->setColor(1.0f, 1.0f, 1.0f);
            particles->setSize(6.0f + counter * .0005f);
            particles->setScale(1.5f + counter * 6.5f);

            glBindTexture(GL_TEXTURE_2D, s.texture[T_EXPLOSION_2]);
            break;

        // explosion fireball
        case OBJ_EXPLOSION_3:
            particles->setAlpha(2.0f - counter * .001f);
            particles->setColor(1.0f, .65f - counter * .000125f, .35f - counter * .000075f);
            particles->setSize(5.0f - counter * .005f);
            particles->setScale(1.0f + counter * 5.0f);

            glBindTexture(GL_TEXTURE_2D, s.texture[T_EXPLOSION_2]);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        // collision sparks
        case OBJ_EXPLOSION_4:
            particles->setAlpha(2.0f - counter * .002f);
            particles->setColor(1.0f, .3f, .15f);
            particles->setSize(1.5f);
            particles->setScale(1.0f + counter * 8.0f);

            glBindTexture(GL_TEXTURE_2D, s.texture[T_STAR]);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        // explosion nova
        case OBJ_EXPLOSION_5:
            glBindTexture(GL_TEXTURE_2D, s.texture[T_EXPLOSION_4]);
            use_particles = false;
            break;
    }

    glRotatef(s.tilt_x * -.035f, 0, 1, 0);
    glRotatef(s.tilt_y * -.035f, 1, 0, 0);

    glShadeModel(GL_FLAT);
    glPushMatrix();

    if (use_particles) {
        particles->draw(
            s,
            (p_x - s.cam_x) * E_RELATIVE_MOVEMENT,
            (p_y - s.cam_y) * E_RELATIVE_MOVEMENT,
            p_z,
            0, 0, r_z
        );
    } else {
        // nova/halo effect without particles

        glTranslatef(
            (p_x - s.cam_x) * E_RELATIVE_MOVEMENT,
            (p_y - s.cam_y) * E_RELATIVE_MOVEMENT,
            (p_z)
        );

        float r = 20.0f + counter * 50.0f;

        glColor4f(1.0f, 1.0f, 1.0f, .5f - counter * .001f);
        glScalef(1.0f, 1.0f, 1.0f);

        glBegin (GL_QUADS);
          glTexCoord2f(0, 1);
          glVertex3f(-r, -r, 0);

          glTexCoord2f(1, 1);
          glVertex3f(r, -r, 0);

          glTexCoord2f(1, 0);
          glVertex3f(r, r, 0);

          glTexCoord2f(0, 0);
          glVertex3f(-r, r, 0);
        glEnd();

        glPopMatrix();
    }

    glPopMatrix();
    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
