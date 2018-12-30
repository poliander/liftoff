#include "explosion.hpp"

Explosion::Explosion(State* sptr) {
    state = sptr;

    particles = new ParticleEngine(state);
    particles->setup(EMITTER_EXPLOSION, 40, .15f, .15f, .1f, 3.0f, 1.25f);
}

Explosion::~Explosion() {
    delete particles;
}

bool Explosion::add(unsigned short eid, float px, float py, float pz, float sp) {
    int life_time = 0;
    object_t new_explosion;

    switch(eid) {
        // green laser gun impact
        case OBJ_EXPLOSION_1:
            life_time = 1000;
            break;

        // explosion smoke
        case OBJ_EXPLOSION_2:
            life_time = 4000;
            break;

        // explosion fireball
        case OBJ_EXPLOSION_3:
            life_time = 2000;
            break;

        // collision sparks
        case OBJ_EXPLOSION_4:
            life_time = 1000;
            break;

        // explosion nova
        case OBJ_EXPLOSION_5:
            life_time = 1500;
            break;
    }

    new_explosion.type        = OBJ_TYPE_EXPLOSION;
    new_explosion.id        = eid;
    new_explosion.waiting    = 0;
    new_explosion.life        = 0;
    new_explosion.life_time    = 0;
    new_explosion.life_max    = life_time;
    new_explosion.cnt        = .0f;
    new_explosion.speed        = sp;

    new_explosion.pos_x        = px;
    new_explosion.pos_y        = py;
    new_explosion.pos_z        = pz;

    new_explosion.rot_x        = float(rand()%360);
    new_explosion.rot_y        = float(rand()%360);
    new_explosion.rot_z        = float(rand()%360);

    new_explosion.rsp_x        = .0f;
    new_explosion.rsp_y        = .0f;
    new_explosion.rsp_z        = .0f;

    new_explosion.scale_x    = .0f;
    new_explosion.scale_y    = .0f;
    new_explosion.scale_z    = .0f;

    return state->add(&new_explosion);
}

void Explosion::move(int oid) {
    if (oid == -1) {
        particles->move();
        return;
    }

    state->objects[oid].life_time += int(state->timer_adjustment*22.5f);

    if (state->objects[oid].life_time >= state->objects[oid].life_max) {
        state->remove(oid);
        return;
    }

    state->objects[oid].cnt += (7.0f - state->objects[oid].cnt * 1.5f) * state->timer_adjustment * .1f;

    state->objects[oid].pos_z += state->timer_adjustment * (state->objects[oid].speed + state->objects[state->player].speed);
    if (state->objects[oid].pos_z > 100)
        state->remove(oid);
}

void Explosion::draw(int oid) {
    bool use_particles = true;

    switch(state->objects[oid].id) {

        // green sparky gun fire impact
        case OBJ_EXPLOSION_1:
            particles->setAlpha(1.5f - (state->objects[oid].life_time * .0015f));
            particles->setColor(.5f, 1.0f, .8f);
            particles->setSize(3.0f);
            particles->setScale(1.0f + state->objects[oid].cnt);
            particles->setParticleNumber(30);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        // explosion smoke
        case OBJ_EXPLOSION_2:
            particles->setAlpha( (.5f - (state->objects[oid].life_time * .0001165f)));
            particles->setColor(1.0f, 1.0f, 1.0f);
            particles->setSize(6.0f + state->objects[oid].life_time * .0005f);
            particles->setScale(1.5f + state->objects[oid].cnt * 6.5f);
            particles->setParticleNumber(20);
            break;

        // explosion fireball
        case OBJ_EXPLOSION_3:
            particles->setAlpha(2.0f - (state->objects[oid].life_time * .001f));
            particles->setColor(1.0f, .65f - (state->objects[oid].life_time * .000125f), .35f - (state->objects[oid].life_time * .000075f));
            particles->setSize(5.0f - (state->objects[oid].life_time * .005f));
            particles->setScale(1.0f + state->objects[oid].cnt * 5.0f);
            particles->setParticleNumber(30);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        // collision sparks
        case OBJ_EXPLOSION_4:
            particles->setAlpha(2.0f - (state->objects[oid].life_time * .002f));
            particles->setColor(1.0f, .3f, .15f);
            particles->setSize(1.5f);
            particles->setScale(1.0f + state->objects[oid].cnt * 8.0f);
            particles->setParticleNumber(20);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        // explosion nova
        case OBJ_EXPLOSION_5:
            use_particles = false;
            break;
    }

    glRotatef(-state->tilt_x*.035f, 0, 1, 0);
    glRotatef(-state->tilt_y*.035f, 1, 0, 0);

    glShadeModel(GL_FLAT);
    glPushMatrix();
    if (use_particles) {
        particles->draw(
            (state->objects[oid].pos_x - state->cam_x) * E_RELATIVE_MOVEMENT,
            (state->objects[oid].pos_y - state->cam_y) * E_RELATIVE_MOVEMENT,
            state->objects[oid].pos_z,
            0, 0, state->objects[oid].rot_z);
    } else {
        // nova/halo effect without particles
        glTranslatef(
            (state->objects[oid].pos_x - state->cam_x) * E_RELATIVE_MOVEMENT,
            (state->objects[oid].pos_y - state->cam_y) * E_RELATIVE_MOVEMENT,
            state->objects[oid].pos_z
        );
        float s = 20.0f + state->objects[oid].cnt * 50.0f;
        glColor4f(1.0f, 1.0f, 1.0f, .5f - state->objects[oid].life_time * .001f);
        glScalef(1.0f, 1.0f, 1.0f);
        glBegin (GL_QUADS);
          glTexCoord2f (0, 1);
          glVertex3f (-s, -s, 0);

          glTexCoord2f (1, 1);
          glVertex3f (s, -s, 0);

          glTexCoord2f (1, 0);
          glVertex3f (s, s, 0);

          glTexCoord2f (0, 0);
          glVertex3f (-s, s, 0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();
    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
