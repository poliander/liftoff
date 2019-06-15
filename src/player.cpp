#include "player.hpp"

Player::Player() : Entity()
{
    e_id = OBJ_PLAYER;
    e_type = OBJ_TYPE_COLLIDER;
    e_state = OBJ_STATE_ACTIVE;

    particles = new ParticleEngine();
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

    // maximum life, life regeneration and regeneration energy draw
    life_max = 250;
    life_reg = 1;
    life_reg_energy = 10;

    // weapons
    gun_power = 10;
    gun_flash[0] = 0;
    gun_flash[1] = 0;

    life = 1;
    energy = 1;
    money = 0;
}

Player::~Player()
{
    delete particles;
}

unsigned short Player::getAcceleration()
{
    return acceleration;
}

unsigned short Player::getMoney()
{
    return money;
}

void Player::setEnergy(int e)
{
    energy = e;
}

int Player::getEnergy()
{
    return energy;
}

int Player::getEnergyMaximum()
{
    return energy_max;
}

int Player::getEnergyRegeneration()
{
    return energy_reg;
}

int Player::getLifeMaximum()
{
    return life_max;
}

int Player::getLifeRegeneration()
{
    return life_reg;
}

int Player::getLifeRegenerationEnergy()
{
    return life_reg_energy;
}

bool Player::isTarget(shared_ptr<Entity> &e)
{
    bool target = false;

/*
    if (
        (p_x < (e->getPosX() + (e->getScale() * 1.25f))) &&
        (p_x > (e->getPosX() - (e->getScale() * 1.25f))) &&
        (p_y < (e->getPosY() + (e->getScale() * 1.25f))) &&
        (p_y > (e->getPosY() - (e->getScale() * 1.25f)))
    ) {
        if (
            state.objects[state.player].target != -1 &&
            state.objects[oid].pos_z > state.objects[state.objects[state.player].target].pos_z
        ) {
            state.objects[state.player].target = oid;
        } else if (state.objects[state.player].target == -1) {
            state.objects[state.player].target = oid;
        }
    } else if (oid == state.objects[state.player].target) {
        state.objects[state.player].target = -1;
    }
*/

    return target;
}

/**
 * player receives item
 */
void Player::collect(unsigned short item)
{
    powerup = item;
}

/*
 * fire primary weapons
 */
void Player::shoot(State &s)
{
    static int m_alt = 0;
    static GLuint m_next_shot = s.timer;
    float drift_x = .0f, drift_y = .0f, delta_z;
    Sint16 angle;

    if (life <= 0) {
        // player dead
        return;
    }

    if (m_next_shot > s.timer) {
        // timing
        return;
    }

//    m_next_shot = s.timer + 90 + rand() % 60;
    m_next_shot = s.timer + 9 + rand() % 6;

    if (energy < 20) {
        // low energy
        return;
    }

//    state.objects[state.player].energy -= 20;

    // left/right alteration, randomize gun flash
    m_alt = 1 - m_alt;
    gun_flash[m_alt] = 1.0f;
    gun_flash_rot[m_alt] = float(rand()%360);

    auto missile = make_shared<Missile>();

    missile->setPos(
        p_x - 8.5f + (m_alt * 17.0f),
        p_y,
        p_z - 150.0f
    );

    s.entities.push_back(missile);

    angle = int(.5f * (p_x - s.cam_x));

    if (angle < 0) {
        angle += 360;
    }

    s.audio.playSample(2, 255, angle);
}

void Player::move(State &s)
{
    float deceleration;

    if (life > 0) {
        deceleration = .00035f;
    } else {
        deceleration = .0002f;
    }

    // check boundary
    if (s.engine_boundary) {
        if (p_x < -600.0f) {
            p_x = -600.0f;

            if (a_x > 0) {
                a_x = 0;
            }
        }

        if (p_x > 600.0f) {
            p_x = 600.0f;

            if (a_x < 0) {
                a_x = 0;
            }
        }

        if (p_y < -400.0f) {
            p_y = -400.0f;

            if (a_y > 0) {
                a_y = 0;
            }
        }

        if (p_y > 400.0f) {
            p_y = 400.0f;

            if (a_y < 0) {
                a_y = 0;
            }
        }
    }

    // limit horizontal acceleration
    if (fabs(a_x) > acceleration * .005f) {
        if (a_x > 0) {
            a_x = acceleration * .005f;
        } else {
            a_x = acceleration * -.005f;
        }
    }

    // limit vertical acceleration
    if (fabs(a_y) > acceleration * .005f) {
        if (a_y > 0) {
            a_y = acceleration * .005f;
        } else {
            a_y = acceleration * -.005f;
        }
    }

    // accelerate horizontally
    if ((a_x - v_x) > .0005f) {
        v_x += (a_x - v_x) * s.timer_adjustment * acceleration * deceleration;
    } else if ((v_x - a_x) > .0005f) {
        v_x -= (v_x - a_x) * s.timer_adjustment * acceleration * deceleration;
    }

    // move horizontally
    p_x -= v_x * s.timer_adjustment * 10.0f;

    // accelerate vertically
    if ((a_y - v_y) > .0005f) {
        v_y += (a_y - v_y) * s.timer_adjustment * acceleration * deceleration;
    } else if ((v_y - a_y) > .0005f) {
        v_y -= (v_y - a_y) * s.timer_adjustment * acceleration * deceleration;
    }

    // move vertically
    p_y -= v_y * s.timer_adjustment * 10.0f;

    a_x = 0;
    a_y = 0;

    // accelerate and move forward/backward
    if (v_z < a_z) {
        v_z += .04f * ((a_z - v_z) + .01f) * s.timer_adjustment;
    } else if (v_z > a_z) {
        v_z -= .04f * ((v_z - a_z) + .01f) * s.timer_adjustment;
    }

    p_z -= v_z * s.timer_adjustment;

    // engine exhausts

    jr += s.timer_adjustment * .2f;

    if (jr > (.3f + (rand()%200)*.001f)) {
        jr = 0;
        j_l = 1.5f+((rand()%40)*.1);
    }

    if (jt_l < j_l) {
        jt_l += .1f * ((j_l - jt_l )+.05f) * s.timer_adjustment;
    }

    if (jt_l > j_l) {
        jt_l -= .1f * ((jt_l - j_l )+.05f) * s.timer_adjustment;
    }

    particles->move(s);

    // rotate
    r_x += s.timer_adjustment * w_x * .1f;
    if (r_x < 0) r_x += 360.0f;
    if (r_x > 360.0f) r_x -= 360.0f;

    r_y += s.timer_adjustment * w_y * .1f;
    if (r_y < 0) r_y += 360.0f;
    if (r_y > 360.0f) r_y -= 360.0f;

    r_z += s.timer_adjustment * w_z * .1f;
    if (r_z < 0) r_z += 360.0f;
    if (r_z > 360.0f) r_z -= 360.0f;

    // camera tilt
    if (s.tilt_factor > .05f) {
        static GLuint next_tilt_impulse = s.timer;

        if (next_tilt_impulse <= s.timer) {
            next_tilt_impulse = s.timer + 100 + rand() % 50;

            s.tilt_dx = -s.tilt_factor + float(rand() % int(s.tilt_factor * 200.0f)) * .0075f;
            s.tilt_dy = -s.tilt_factor + float(rand() % int(s.tilt_factor * 200.0f)) * .0075f;
        }

        s.tilt_factor -= s.timer_adjustment * .35f;

        s.tilt_x += (.025f + (s.tilt_dx - s.tilt_x)) * s.timer_adjustment * .15f;
        s.tilt_y += (.025f + (s.tilt_dy - s.tilt_y)) * s.timer_adjustment * .15f;
    } else {
        s.tilt_factor = .0f;
        s.tilt_dx = .0f;
        s.tilt_dy = .0f;

        if (fabs(s.tilt_x) > .05f) {
            s.tilt_x += (.025f + (s.tilt_dx - s.tilt_x)) * s.timer_adjustment * .15f;
        } else {
            s.tilt_x = .0f;
        }

        if (fabs(s.tilt_y) > .05f) {
            s.tilt_y += (.025f + (s.tilt_dy - s.tilt_y)) * s.timer_adjustment * .15f;
        } else {
            s.tilt_y = .0f;
        }
    }

    // gun flash animation
    if (gun_flash[0] > 0) gun_flash[0] -= s.timer_adjustment * .2f;
    if (gun_flash[1] > 0) gun_flash[1] -= s.timer_adjustment * .2f;

    if (life <= 0) {
        return;
    }

    // power-ups
    switch (powerup) {
        case OBJ_POWERUP_0:
            powerup_booster_length += 20;
            powerup_booster_ltimer = SDL_GetTicks();
            powerup = 0;
            break;
    }

    powerup_booster_timer += (SDL_GetTicks() - powerup_booster_ltimer);
    powerup_booster_ltimer = SDL_GetTicks();

    // one tick every 0.25s
    if (powerup_booster_timer > 250) {
        powerup_booster_timer -= 250;

        if (life <= 0) {
            energy -= int(ceil((float)energy_max * .05f));

            if (energy <= 0) {
                s.set(STATE_GAME_QUIT);
            }
        } else {
            // life booster
            if (powerup_booster_length > 0) {
                powerup_booster_length--;

                life += int(ceil((float)life_max * .075f));
            }

            // energy regeneration
            energy += energy_reg;

            // life regeneration
            if (life < life_max) {
                if (energy > life_reg_energy) {
                    life += life_reg;

                    if (powerup_booster_length <= 0) {
                        energy -= life_reg_energy;
                    }
                }
            }
        }
    }

    if (life > life_max) {
        life = life_max;
    }

    if (energy > energy_max) {
        energy = energy_max;
    }
}

void Player::draw(State &s)
{
    float alpha, jlen;
    GLfloat lightpos[] = { 0, 0, 0, 1.0f };

    if (s.get() > 10 && s.get() < 20) {
        alpha = s.global_alpha * .01f;
        lightpos[0] = 100.0f;
        lightpos[1] = 1000.0f;
        lightpos[2] = -3000.0f;
    } else {
        alpha = s.title_ypos * .01f;
        lightpos[0] = 50.0f;
        lightpos[1] = 500.0f;
        lightpos[2] = .0f;
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

    if (s.get() <= 10 || s.get() >= 20) {
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

    if (s.get() > 10 && s.get() < 20) {
        // ingame
         glTranslatef(
          ((p_x - s.cam_x) * E_RELATIVE_MOVEMENT) + s.tilt_x * .15f,
          ((p_y - s.cam_y) * E_RELATIVE_MOVEMENT) + s.tilt_y * .15f,
          ((p_z + (s.tilt_x + s.tilt_y) * .15f))
        );
        glScalef(22.5f, 22.5f, 23.5f);
    } else {
        // menu
        glTranslatef(p_x, p_y, p_z - 20.0f);
        glScalef(1.0f, 1.0f, 1.1f);
    }

    // rotation including pitch/roll
    glRotatef(r_x + v_y * -20.0f, 1.0f,  .0f,  .0f);
    glRotatef(r_y + v_x *  50.0f,  .0f, 1.0f,  .0f);
    glRotatef(r_z,                 .0f,  .0f, 1.0f);

    glCallList(*s.models[OBJ_PLAYER]);

    if (s.get() <= 10) {
        glDisable(GL_LIGHT1);
    }

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    // flashing gun fire

    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glBindTexture(GL_TEXTURE_2D, *s.textures[T_GLOW_1]);

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
    if ((s.get() > 10) && (s.get() < 20) && (life > 0)) {
        glBindTexture(GL_TEXTURE_2D, *s.textures[T_JET]);

        if (s.get() == STATE_GAME_NEXTLEVEL) {
            particles->setSize(10.0f + s.title_ypos * .025f);
            particles->setScale(1.0f + s.title_ypos * .015f);
        } else {
            particles->setSize(10.0f);
            particles->setScale(1.0f);
        }

        // engine jets
        particles->move(s);
        particles->draw(s, -4.25f, -.75f, .5f, .0f, -90.0f, -5.0f);
        particles->draw(s, -4.25f, .75f, .5f, .0f, -90.0f, 5.0f);

        // exhaust textures
        glRotatef(90, 1, 0, 0);
        glRotatef(-90, 0, 1, 0);
        glTranslatef(0, .5f, 3.25f);
        glBindTexture(GL_TEXTURE_2D, *s.textures[T_JET_EXHAUST]);
        for (int j=1; j<6; j++) {
            jlen = 3.0f + jt_l*j*.25f;

            glTranslatef(-.7f-j*.05f, 0, (((int)s.lvl_pos % 3))*.03f*(j-1));
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
