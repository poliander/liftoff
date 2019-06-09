#include "player.hpp"

Player::Player(State* sptr) : Object(sptr) {
    state = sptr;
    memset(&model, 0, sizeof(struct obj_model_t));

    particles = new ParticleEngine(state);
    particles->setup(EMITTER_JET, 50, 0, 0, .35f, 1.0f, 10.0f);

    jr = 0;
    j_l = 3;
    jt_l = 3;

    powerup_booster_timer = 0;
    powerup_booster_length = 0;

    // speed/acceleration
    acceleration = 120;

    // energy capacity and regeneration speed
    energy_max = 1500;
    energy_reg = 15;

    // shield capacity, regeneration speed and energy usage
    shield_max = 250;
    shield_reg = 1;
    shield_reg_energy = 10;

    // weapons
    gun_power = 10;
    gun_flash[0] = 0;
    gun_flash[1] = 0;
}

Player::~Player() {
    delete particles;
}

int Player::getTarget(int oid) {
    if (oid != -1) {
        if ( (state->objects[state->player].pos_x < (state->objects[oid].pos_x + (state->objects[oid].scale_x*1.25f))) &&
             (state->objects[state->player].pos_x > (state->objects[oid].pos_x - (state->objects[oid].scale_x*1.25f))) &&
             (state->objects[state->player].pos_y < (state->objects[oid].pos_y + (state->objects[oid].scale_y*1.25f))) &&
             (state->objects[state->player].pos_y > (state->objects[oid].pos_y - (state->objects[oid].scale_y*1.25f))) ) {

            // new target better than current?
            if ( (state->objects[state->player].target != -1) &&
                 (state->objects[oid].pos_z > state->objects[state->objects[state->player].target].pos_z)) {
                state->objects[state->player].target = oid;
            } else if (state->objects[state->player].target == -1) {
                state->objects[state->player].target = oid;
            }
        } else if (oid == state->objects[state->player].target) {
            state->objects[state->player].target = -1;
        }
    }

    return state->objects[state->player].target;
}

void Player::setTarget(int oid) {
    state->objects[state->player].target = oid;
}

/*
 *     camera shaking ("tilt")
 */
void Player::tilt(float t) {
    if (state->tilt_factor < fabs(t)) {
        state->tilt_factor = fabs(t);
    }
}

/*
 *     fire primary weapons
 */
bool Player::shoot() {
    static int m_alt = 0;
    static GLuint m_next_shot = state->timer;
    float drift_x = .0f, drift_y = .0f, delta_z;
    Sint16 angle;

    // timing
    if (m_next_shot > state->timer) {
        return false;
    } else {
        m_next_shot = state->timer + 90 + rand() % 60;
    }

    // don't fire when player is dead or energy is too low
    if ((state->objects[state->player].life <= 0) || (state->objects[state->player].energy < 20)) {
        return false;
    } else {
        state->objects[state->player].energy -= 20;
    }

    // left/right alteration, randomize gun flash
    m_alt = 1 - m_alt;
    gun_flash[m_alt] = 1.0f;
    gun_flash_rot[m_alt] = float(rand()%360);

    // a little auto aiming for locked targets
    if (state->objects[state->player].target != -1) {
        delta_z = .01f * fabs(state->objects[state->objects[state->player].target].pos_z - state->objects[state->player].pos_z);

        drift_x = -(state->objects[state->player].pos_x - state->objects[state->objects[state->player].target].pos_x);
        drift_y = -(state->objects[state->player].pos_y - state->objects[state->objects[state->player].target].pos_y);

        drift_x = drift_x / delta_z;
        if (drift_x > 2.0f) drift_x = 2.0f;
        if (drift_x < -2.0f) drift_x = -2.0f;

        drift_y = drift_y / delta_z;
        if (drift_y > 2.0f) drift_y = 2.0f;
        if (drift_y < -2.0f) drift_y = -2.0f;
    }

    object_t new_missile;

    new_missile.type        = OBJ_TYPE_MISSILE;
    new_missile.id          = OBJ_MISSILE_1;
    new_missile.waiting     = false;

    new_missile.life        = -1;
    new_missile.life_max    = -1;
    new_missile.life_time   = -1;
    new_missile.cnt         = .0f;
    new_missile.speed       = -125.0f;

    new_missile.pos_x       = state->objects[state->player].pos_x - 8.5f + (m_alt * 17.0f);
    new_missile.pos_y       = state->objects[state->player].pos_y;
    new_missile.pos_z       = state->objects[state->player].pos_z - 150.0f;

    new_missile.rot_x       = .0f;
    new_missile.rot_y       = .0f;
    new_missile.rot_z       = .0f;

    new_missile.rsp_x       = drift_x;
    new_missile.rsp_y       = drift_y;
    new_missile.rsp_z       = .0f;

    new_missile.scale_x     = 2.25f;
    new_missile.scale_y     = 2.25f;
    new_missile.scale_z     = 2.25f;

    state->add(&new_missile);

    angle = int(.5f * (state->objects[state->player].pos_x - state->cam_x));
    if (angle < 0) angle += 360;
    state->audio->playSample(2, 255, angle);

    return true;
}

void Player::move() {
    Uint32 t = SDL_GetTicks();
    float countersteering = .00035f;

    if (state->objects[state->player].life <= 0) {
        countersteering = .0002f;
    }

    // check boundary
    if (state->engine_boundary) {
        if (state->objects[state->player].pos_x < -600.0f) {
            state->objects[state->player].pos_x = -600.0f;
            if (state->objects[state->player].t_x > 0) {
                state->objects[state->player].t_x = 0;
            }
        }

        if (state->objects[state->player].pos_x > 600.0f) {
            state->objects[state->player].pos_x = 600.0f;
            if (state->objects[state->player].t_x < 0) {
                state->objects[state->player].t_x = 0;
            }
        }

        if (state->objects[state->player].pos_y < -400.0f) {
            state->objects[state->player].pos_y = -400.0f;
            if (state->objects[state->player].t_y > 0) {
                state->objects[state->player].t_y = 0;
            }
        }

        if (state->objects[state->player].pos_y > 400.0f) {
            state->objects[state->player].pos_y = 400.0f;
            if (state->objects[state->player].t_y < 0) {
                state->objects[state->player].t_y = 0;
            }
        }
    }

    // horizontal acceleration limit
    if (fabs(state->objects[state->player].t_x) > acceleration * .005f) {
        if (state->objects[state->player].t_x > 0) {
            state->objects[state->player].t_x = acceleration * .005f;
        } else {
            state->objects[state->player].t_x = -acceleration * .005f;
        }
    }

    // vertical acceleration limit
    if (fabs(state->objects[state->player].t_y) > acceleration * .005f) {
        if (state->objects[state->player].t_y > 0) {
            state->objects[state->player].t_y = acceleration * .005f;
        } else {
            state->objects[state->player].t_y = -acceleration * .005f;
        }
    }

    // accelerate and move left/right
    if ((state->objects[state->player].t_x - state->objects[state->player].a_x) > .0005f) {
        state->objects[state->player].a_x += (state->objects[state->player].t_x - state->objects[state->player].a_x) * state->timer_adjustment * acceleration * countersteering;
    } else if ((state->objects[state->player].a_x - state->objects[state->player].t_x) > .0005f) {
        state->objects[state->player].a_x -= (state->objects[state->player].a_x - state->objects[state->player].t_x) * state->timer_adjustment * acceleration * countersteering;
    }
    state->objects[state->player].t_x = 0;
    state->objects[state->player].pos_x -= state->objects[state->player].a_x * state->timer_adjustment * 10.0f;

    // accelerate and move up/down
    if ((state->objects[state->player].t_y - state->objects[state->player].a_y) > .0005f) {
        state->objects[state->player].a_y += (state->objects[state->player].t_y - state->objects[state->player].a_y) * state->timer_adjustment * acceleration * countersteering;
    } else if ((state->objects[state->player].a_y - state->objects[state->player].t_y) > .0005f) {
        state->objects[state->player].a_y -= (state->objects[state->player].a_y - state->objects[state->player].t_y) * state->timer_adjustment * acceleration * countersteering;
    }
    state->objects[state->player].t_y = 0;
    state->objects[state->player].pos_y -= state->objects[state->player].a_y * state->timer_adjustment * 10.0f;

    // accelerate and move forward/backward
    if (state->objects[state->player].a_z < state->objects[state->player].t_z) {
        state->objects[state->player].a_z += .04f * ((state->objects[state->player].t_z - state->objects[state->player].a_z)+.01f) * state->timer_adjustment;
    } else if (state->objects[state->player].a_z > state->objects[state->player].t_z) {
        state->objects[state->player].a_z -= .04f * ((state->objects[state->player].a_z - state->objects[state->player].t_z)+.01f) * state->timer_adjustment;
    }
    state->objects[state->player].pos_z -= state->objects[state->player].a_z * state->timer_adjustment;

    // engine exhausts
    jr += state->timer_adjustment * .2f;
    if (jr > (.3f + (rand()%200)*.001f)) {
        jr = 0;
        j_l = 1.5f+((rand()%40)*.1);
    }
    if (jt_l < j_l) {
        jt_l += .1f * ((j_l - jt_l )+.05f) * state->timer_adjustment;
    }
    if (jt_l > j_l) {
        jt_l -= .1f * ((jt_l - j_l )+.05f) * state->timer_adjustment;
    }
    particles->move();

    // spin rotation
    setRot(
        state->player,
        getRotX(state->player) + getSpinX(state->player) * state->timer_adjustment,
        getRotY(state->player) + getSpinY(state->player) * state->timer_adjustment,
        getRotZ(state->player) + getSpinZ(state->player) * state->timer_adjustment
    );

    // camera tilt
    if (state->tilt_factor > .05f) {
        static GLuint next_tilt_impulse = state->timer;
        if (next_tilt_impulse <= state->timer) {
            next_tilt_impulse = state->timer + 100 + rand() % 50;

            state->tilt_dx = -state->tilt_factor + float(rand() % int(state->tilt_factor * 200.0f)) * .0075f;
            state->tilt_dy = -state->tilt_factor + float(rand() % int(state->tilt_factor * 200.0f)) * .0075f;
        }

        state->tilt_factor -= state->timer_adjustment * .35f;

        state->tilt_x += (.025f + (state->tilt_dx - state->tilt_x)) * state->timer_adjustment * .15f;
        state->tilt_y += (.025f + (state->tilt_dy - state->tilt_y)) * state->timer_adjustment * .15f;
    } else {
        state->tilt_factor = .0f;
        state->tilt_dx = .0f;
        state->tilt_dy = .0f;
        if (fabs(state->tilt_x) > .05f) {
            state->tilt_x += (.025f + (state->tilt_dx - state->tilt_x)) * state->timer_adjustment * .15f;
        } else {
            state->tilt_x = .0f;
        }
        if (fabs(state->tilt_y) > .05f) {
            state->tilt_y += (.025f + (state->tilt_dy - state->tilt_y)) * state->timer_adjustment * .15f;
        } else {
            state->tilt_y = .0f;
        }
    }

    // gun flash animation
    if (gun_flash[0] > 0) gun_flash[0] -= state->timer_adjustment * .2f;
    if (gun_flash[1] > 0) gun_flash[1] -= state->timer_adjustment * .2f;

    if (state->objects[state->player].life <= 0) {
        return;
    }

    // power-ups
    if (state->objects[state->player].powerup) {
        switch(state->objects[state->player].powerup) {
            case OBJ_POWERUP_0:
                powerup_booster_length += 20;
                powerup_booster_ltimer = SDL_GetTicks();
                state->objects[state->player].powerup = 0;
                break;
        }

        state->objects[state->player].powerup = 0;
    }

    powerup_booster_timer += (t - powerup_booster_ltimer);
    powerup_booster_ltimer = t;

    // one tick every 0.25s
    if (powerup_booster_timer > 250) {
        powerup_booster_timer -= 250;

        if (state->objects[state->player].life <= 0) {
            state->objects[state->player].energy -= int(ceil((float)energy_max * .05f));

            // player
            if (state->objects[state->player].energy < 0) {
                state->objects[state->player].energy = 0;
                state->set(STATE_GAME_QUIT);
            }
        } else {
            // shield booster
            if (powerup_booster_length > 0) {
                powerup_booster_length--;

                state->objects[state->player].life += int(ceil((float)shield_max * .075f));
            }

            // energy regeneration
            state->objects[state->player].energy += energy_reg;

            // shield regeneration
            if (state->objects[state->player].life < shield_max) {
                if (state->objects[state->player].energy > shield_reg_energy) {
                    state->objects[state->player].life += shield_reg;
                    if (powerup_booster_length <= 0) {
                        state->objects[state->player].energy -= shield_reg_energy;
                    }
                }
            }
        }
    }

    if (state->objects[state->player].life > shield_max)
        state->objects[state->player].life = shield_max;

    if (state->objects[state->player].energy > energy_max)
        state->objects[state->player].energy = energy_max;
}

void Player::draw() {
    float alpha, jlen;
    GLfloat lightpos[] = { 100.0f, 0, 0, 1.0f };

    if (state->get() <= 10) {
        alpha = state->title_ypos * .01f;
        lightpos[0] = 50.0f;
        lightpos[1] = 500.0f;
        lightpos[2] = .0f;
    } else {
        alpha = state->global_alpha * .01f;
        lightpos[1] = 1000.0f;
        lightpos[2] = -3000.0f;
    }

    GLfloat lightcol[] = { 1, 1, 1, alpha };

    GLfloat mat_diffuse[] = { alpha*.05f, alpha*.05f, alpha*.05f, alpha };
    GLfloat col_diffuse[] = { .0f, .0f, .0f };

    GLfloat mat_emission[] = { .15f*alpha, .2f*alpha, .2f*alpha, alpha };
    GLfloat mat_fire[] = { .15f*alpha, .15f*alpha, .15f*alpha, alpha };

    GLfloat col_specular[] = { alpha, alpha, alpha, 0 };
    GLfloat mat_specular[] = { alpha, alpha, alpha, 0 };

    glLoadIdentity();

    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_SPECULAR, col_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightcol);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, col_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glEnable(GL_LIGHT0);

    if (state->get() <= 10) {
        lightpos[0] = -150.0f;
        lightpos[1] = 300.0f;
        lightpos[2] = -300.0f;
        glLightfv(GL_LIGHT1, GL_SPECULAR, col_specular);
        glLightfv(GL_LIGHT1, GL_AMBIENT, lightcol);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, col_diffuse);
        glLightfv(GL_LIGHT1, GL_POSITION, lightpos);
        glEnable(GL_LIGHT1);
    }

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_fire);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);

    glPushMatrix();

    if (state->get() > 10) {
        // ingame
        glTranslatef(
          ((state->objects[state->player].pos_x - state->cam_x) * E_RELATIVE_MOVEMENT) + state->tilt_x * .15f,
          ((state->objects[state->player].pos_y - state->cam_y) * E_RELATIVE_MOVEMENT) + state->tilt_y * .15f,
           (state->objects[state->player].pos_z + (state->tilt_x + state->tilt_y) * .15f)
        );
        glScalef(22.5f, 22.5f, 23.5f);
    } else {
        // menu
        glTranslatef(
          state->objects[state->player].pos_x,
          state->objects[state->player].pos_y,
          state->objects[state->player].pos_z - 20.0f
        );
        glScalef(1.0f, 1.0f, 1.1f);
    }

    // roll
    glRotatef(270.0f + (state->objects[state->player].a_x * 50.0f), 0, 0, 1);

    // pitch
    glRotatef(state->objects[state->player].a_y * -20.0f, 0, 1, 0);

    glRotatef(getRotX(state->player), 0, 1, 0);
    glRotatef(getRotY(state->player), 0, 0, 1);
    glRotatef(getRotZ(state->player), 1, 0, 0);
    glRotatef(90, 0, 1, 0);

    glCallList(model_list);

    if (state->get() <= 10) {
        glDisable(GL_LIGHT1);
    }

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    // flashing gun fire
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    if (gun_flash[0] > 0) {
        glTranslatef(1.5f, -1.0f, .5f);
        glRotatef(gun_flash_rot[0], 1, 0, 0);
        glColor4f(.5f, 1.0f, .8f, gun_flash[0]);
        glBegin (GL_QUADS);
          glTexCoord2f(0, 0);
          glVertex3f(0, -1.25f, -1.25f);

          glTexCoord2f(1, 0);
          glVertex3f(0, 1.25f, -1.25f);

          glTexCoord2f(1, 1);
          glVertex3f(0, 1.25f, 1.25f);

          glTexCoord2f(0, 1);
          glVertex3f(0, -1.25f, 1.25f);
        glEnd();
        glRotatef(gun_flash_rot[0], -1, 0, 0);
        glTranslatef(-1.5f, 1.0f, -.5f);
    }
    if (gun_flash[1] > 0) {
        glTranslatef(1.5f, 1.0f, .5f);
        glRotatef(gun_flash_rot[1], 1, 0, 0);
        glColor4f(.5f, 1.0f, .8f, gun_flash[1]);
        glBegin (GL_QUADS);
          glTexCoord2f(0, 0);
          glVertex3f(0, -1.25f, -1.25f);

          glTexCoord2f(1, 0);
          glVertex3f(0, 1.25f, -1.25f);

          glTexCoord2f(1, 1);
          glVertex3f(0, 1.25f, 1.25f);

          glTexCoord2f(0, 1);
          glVertex3f(0, -1.25f, 1.25f);
        glEnd();
        glRotatef(gun_flash_rot[1], -1, 0, 0);
        glTranslatef(-1.5f, -1.0f, -.5f);
    }

    glDisable(GL_DEPTH_TEST);

    // jet
    if ( (state->get() > 10) && (state->objects[state->player].life > 0) ) {
        glBindTexture(GL_TEXTURE_2D, textures[3]);

        if (state->get() == STATE_GAME_NEXTLEVEL) {
            particles->setAlpha(.85f + state->title_ypos * .005f);
            particles->setScale(30.0f + state->title_ypos * .5f);
            particles->setSize(.3f + state->title_ypos * .0075f);
            particles->setDirection(0, 0, .3f + state->title_ypos * .0025f);
        }

        // engine jets
        particles->move();
        particles->draw(-4.25f, -.75f, .5f, .0f, -90.0f, -5.0f);
        particles->draw(-4.25f, .75f, .5f, .0f, -90.0f, 5.0f);

        // exhaust textures
        glRotatef(90, 1, 0, 0);
        glRotatef(-90, 0, 1, 0);
        glTranslatef(0, .5f, 3.25f);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        for (int j=1; j<6; j++) {
            jlen = 3.0f + jt_l*j*.25f;

            glTranslatef(-.7f-j*.05f, 0, (((int)state->lvl_pos % 3))*.03f*(j-1));
            glRotatef(j*2, 0, 0, 1);
            glBegin (GL_QUADS);
                glColor4f(.8f, .8f, 1, 0);
                glTexCoord2f(0, 0);
                glVertex3f(-.25, -.125f+j*.05f, .6f);

                glTexCoord2f(1, 0);
                glVertex3f(.25, 0, .6f);

                glColor4f(1-(.075f*j*jt_l), 1-(.05f*j*jt_l), 1-(.01f*j*jt_l), alpha*(1-j*.15f));
                glTexCoord2f(1, .95f);
                glVertex3f(.15, 0, jlen);

                glTexCoord2f(0, .95f);
                glVertex3f(-.25, 0, jlen);
            glEnd();

            glRotatef(-j*2, 0, 0, 1);

            glTranslatef(1.4f+j*.1f, 0, 0);
            glRotatef(-j*2, 0, 0, 1);
            glBegin (GL_QUADS);
                glColor4f(.8f, .8f, 1, 0);
                glTexCoord2f(0, 0);
                glVertex3f(-.25, 0, .6f);

                glTexCoord2f(1, 0);
                glVertex3f(.25, -.125f+j*.05f, .6f);

                glColor4f(1-(.075f*j*jt_l), 1-(.05f*j*jt_l), 1-(.01f*j*jt_l), alpha*(1-j*.15f));
                glTexCoord2f(1, .95f);
                glVertex3f(.15, 0, jlen);

                glTexCoord2f(0, .95f);
                glVertex3f(-.25, 0, jlen);
            glEnd();
            glRotatef(j*2, 0, 0, 1);

            glTranslatef(-.7f-j*.05f, 0, 0);
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPopMatrix();
}
