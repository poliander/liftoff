#include "player.hpp"

Player::Player() : Entity()
{
    e_obj = OBJ_PLAYER;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_ACTIVE;

    jr = 0;
    j_l = 3;
    jt_l = 3;

    s_x = 22.5f;
    s_y = 22.5f;
    s_z = 22.5f;

    // stats
    acceleration = 120;
    energy_max = 1500;
    energy_reg = 10;
    life_max = 250;
    life_reg = 1;
    life_reg_energy = 10;

    // weapons
    gun_power = 10;
    gun_flash[0] = 0;
    gun_flash[1] = 0;
}

Player::~Player()
{
}

void Player::collect(unsigned short e_obj)
{
    powerup = e_obj;
}

void Player::collide(State &s, shared_ptr<Entity> e)
{
}

void Player::init(State &s)
{
    setPos(0, -90.0f, 50.0f);
    setRot(90.0f, 0, 270.0f);
    setSpin(0, 0, 0);
    setVelocity(0, 0, 0);
    setAcceleration(0, 0, 0);

    setMoney(0);
    setEnergy(-200);
    setLife(getLifeMaximum());

    m_alt = 0;
    m_next_shot = s.timer;

    powerup_booster_timer = 0;
    powerup_booster_length = 0;

    collect(OBJ_POWERUP_0);
}

void Player::shoot(State &s)
{
    float ax, ay, dx, dy, dz, hx, hy;
    Sint16 angle;

    if (life <= 0) {
        // player dead
        return;
    }

    if (m_next_shot > s.timer) {
        // timing
        return;
    }

    m_next_shot = s.timer + 90 + rand() % 60;

    if (energy < 25) {
        // low energy
        return;
    }

    energy -= 25;
    m_alt = 1 - m_alt;

    gun_flash[m_alt] = 1.0f;
    gun_flash_rot[m_alt] = float(rand() % 360);

    auto missile = make_shared<Missile>();

    missile->setPos(
        p_x - 10.0f + (m_alt * 20.0f),
        p_y,
        p_z - 50.0f
    );

    if (target && false == target->isCollectable()) {
        dx = getPosX() - target->getPosX();
        dy = getPosY() - target->getPosY();
        dz = getPosZ() - target->getPosZ();

        ax = dx / dz;
        ay = dy / dz;

        missile->setVelocityX(-ax * E_BASE_SPEED * 5.0f);
        missile->setVelocityY(-ay * E_BASE_SPEED * 5.0f);
    }

    s.spawn(missile);

    angle = int(.5f * (p_x - s.cam_x));

    if (angle < 0) {
        angle += 360;
    }

    s.audio.playSample(2, 255, angle);
}

void Player::update(State &s)
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
        s.tilt_factor = 0;
        s.tilt_dx = 0;
        s.tilt_dy =0;

        if (fabs(s.tilt_x) > .05f) {
            s.tilt_x += (.025f + (s.tilt_dx - s.tilt_x)) * s.timer_adjustment * .15f;
        } else {
            s.tilt_x = 0;
        }

        if (fabs(s.tilt_y) > .05f) {
            s.tilt_y += (.025f + (s.tilt_dy - s.tilt_y)) * s.timer_adjustment * .15f;
        } else {
            s.tilt_y = 0;
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
            powerup_booster_length += 15;
            powerup_booster_ltimer = SDL_GetTicks();
            powerup = 0;
            break;

        case OBJ_POWERUP_1:
            powerup_booster_length += 5;
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
            // life/energy boost
            if (powerup_booster_length > 0) {
                powerup_booster_length--;

                life += int(ceil((float)life_max * .1f));
                energy += int(ceil((float)energy_max * .1f));
            } else {
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
    float jlen, a = s.global_alpha * .01f;

    if (
        s.get() < STATE_GAME_LOOP ||
        s.get() > STATE_GAME_QUIT
    ) {
        a = s.menu_title_pos * .01f;
    }

    glm::mat4 projection = s.view->getProjection();
    glm::mat4 camera = s.view->getCamera();
    glm::mat4 model = s.view->getModel(
        (getPosX() - s.cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - s.cam_y) * E_RELATIVE_MOVEMENT,
        getPosZ(),

        getRotX() + v_y * -20.0f,
        getRotY() + v_x *  50.0f,
        getRotZ(),

        getScaleX(),
        getScaleY(),
        getScaleZ()
    );

    // flashing gun fire

    if (gun_flash[0] > 0) {
        glm::mat4 m;

        m = glm::translate(model, glm::vec3(0.5f, -1.25f, 0.5f));
        m = glm::rotate(m, glm::radians(90.0f), glm::vec3(0, 1.0f, 0));
        m = glm::rotate(m, glm::radians(gun_flash_rot[0]), glm::vec3(0, 0, 1.0f));
        m = glm::scale(m, glm::vec3(2.5f, 2.5f, 0));

        s.shaders[S_TEXTURE]->bind();
        s.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.5f * a, 1.0f * a, .7f * a, gun_flash[0] * a));
        s.shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);

        glDepthMask(GL_FALSE);
        s.textures[T_GLOW]->bind();
        s.textures[T_GLOW]->draw();
        glDepthMask(GL_TRUE);

       s.shaders[S_TEXTURE]->unbind();
    }

    if (gun_flash[1] > 0) {
        glm::mat4 m;
        
        m = glm::translate(model, glm::vec3(0.5f, 1.25f, 0.5f));
        m = glm::rotate(m, glm::radians(90.0f), glm::vec3(0, 1.0f, 0));
        m = glm::rotate(m, glm::radians(gun_flash_rot[1]), glm::vec3(0, 0, 1.0f));
        m = glm::scale(m, glm::vec3(2.5f, 2.5f, 0));

        s.shaders[S_TEXTURE]->bind();
        s.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.5f * a, 1.0f * a, .7f * a, gun_flash[1] * a));
        s.shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);

        glDepthMask(GL_FALSE);
        s.textures[T_GLOW]->bind();
        s.textures[T_GLOW]->draw();
        glDepthMask(GL_TRUE);

       s.shaders[S_TEXTURE]->unbind();
    }

    s.models[e_obj]->draw(model, camera, projection, glm::vec4(c_r * a, c_g * a, c_b * a, 1.0f));
}
