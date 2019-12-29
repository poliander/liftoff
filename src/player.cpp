#include "player.hpp"

Player::Player(State &s) : Entity(s) {
    e_obj = OBJ_PLAYER;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_ACTIVE;

    init();
}

Player::~Player() {
}

void Player::init() {
    setPos(0, -90.0f, 50.0f);
    setRot(90.0f, 0, 270.0f);
    setScale(22.5f, 22.5f, 22.5f);
    setSpin(0, 0, 0);
    setVelocity(0, 0, 0);
    setAcceleration(0, 0, 0);

    m_alt = 0;
    m_next_shot_timer = SDL_GetTicks();
    powerup_timer = SDL_GetTicks();

    gun_flash[0] = 0;
    gun_flash[1] = 0;

    // player stats

    acceleration = 120;
    deceleration = 0.00035f;

    powerup = 0;
    money = 0;
    gun_power = 25;

    energy = -200;
    energy_max = 1500;
    energy_reg = 10;

    life = 100;
    life_max = life;
    life_reg = 1;
    life_reg_energy = 10;

    collect(OBJ_POWERUP_0);
}

void Player::collect(uint16_t e_obj) {
    powerup_timer = SDL_GetTicks();

    switch (e_obj) {
        case OBJ_POWERUP_0:
            powerup += 15;
            break;

        case OBJ_POWERUP_1:
            powerup += 5;
            break;
    }
}

void Player::collide(shared_ptr<Entity> e) {
}

void Player::shoot() {
    float ax, ay, dx, dy, dz, hx, hy;
    Sint16 angle;

    if (life <= 0) {
        // player dead
        return;
    }

    if (m_next_shot_timer > SDL_GetTicks()) {
        // timing
        return;
    }

    m_next_shot_timer = SDL_GetTicks() + 90 + rand() % 60;

    if (energy < 25) {
        // low energy
        return;
    }

    energy -= 25;
    m_alt = 1 - m_alt;

    gun_flash[m_alt] = 1.0f;
    gun_flash_rot[m_alt] = static_cast<float>(rand() % 360);

    auto missile = make_shared<Missile>(state, gun_power);

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

    state.spawn(missile);

    angle = static_cast<int>(.5f * (p_x - state.cam_x));

    if (angle < 0) {
        angle += 360;
    }

    state.audio.playSample(2, 255, angle);
}

void Player::update() {
    // check boundary
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
        v_x += (a_x - v_x) * state.global_timer * acceleration * deceleration;
    } else if ((v_x - a_x) > .0005f) {
        v_x -= (v_x - a_x) * state.global_timer * acceleration * deceleration;
    }

    // move horizontally
    p_x -= v_x * state.global_timer * 10.0f;

    // accelerate vertically
    if ((a_y - v_y) > .0005f) {
        v_y += (a_y - v_y) * state.global_timer * acceleration * deceleration;
    } else if ((v_y - a_y) > .0005f) {
        v_y -= (v_y - a_y) * state.global_timer * acceleration * deceleration;
    }

    // move vertically
    p_y -= v_y * state.global_timer * 10.0f;

    a_x = 0;
    a_y = 0;

    // accelerate and move forward/backward
    if (v_z < a_z) {
        v_z += .04f * ((a_z - v_z) + .01f) * state.global_timer;
    } else if (v_z > a_z) {
        v_z -= .04f * ((v_z - a_z) + .01f) * state.global_timer;
    }

    p_z -= v_z * state.global_timer;

    // rotate
    r_x += state.global_timer * w_x * .1f;
    if (r_x < 0) r_x += 360.0f;
    if (r_x > 360.0f) r_x -= 360.0f;

    r_y += state.global_timer * w_y * .1f;
    if (r_y < 0) r_y += 360.0f;
    if (r_y > 360.0f) r_y -= 360.0f;

    r_z += state.global_timer * w_z * .1f;
    if (r_z < 0) r_z += 360.0f;
    if (r_z > 360.0f) r_z -= 360.0f;

    // camera tilt
    if (state.tilt_factor > .05f) {
        static GLuint next_tilt_impulse = SDL_GetTicks();

        if (next_tilt_impulse < SDL_GetTicks()) {
            next_tilt_impulse = SDL_GetTicks() + 100 + rand() % 50;

            state.tilt_dx = -state.tilt_factor + static_cast<float>(rand() % static_cast<int>(state.tilt_factor * 200.0f)) * .0075f;
            state.tilt_dy = -state.tilt_factor + static_cast<float>(rand() % static_cast<int>(state.tilt_factor * 200.0f)) * .0075f;
        }

        state.tilt_factor -= state.global_timer * .35f;

        state.tilt_x += (.025f + (state.tilt_dx - state.tilt_x)) * state.global_timer * .15f;
        state.tilt_y += (.025f + (state.tilt_dy - state.tilt_y)) * state.global_timer * .15f;
    } else {
        state.tilt_factor = 0;
        state.tilt_dx = 0;
        state.tilt_dy =0;

        if (fabs(state.tilt_x) > .05f) {
            state.tilt_x += (.025f + (state.tilt_dx - state.tilt_x)) * state.global_timer * .15f;
        } else {
            state.tilt_x = 0;
        }

        if (fabs(state.tilt_y) > .05f) {
            state.tilt_y += (.025f + (state.tilt_dy - state.tilt_y)) * state.global_timer * .15f;
        } else {
            state.tilt_y = 0;
        }
    }

    // gun flash animation
    if (gun_flash[0] > 0) gun_flash[0] -= state.global_timer * .15f;
    if (gun_flash[1] > 0) gun_flash[1] -= state.global_timer * .15f;

    // one tick every 0.25s
    if (SDL_GetTicks() - powerup_timer > E_TICK_TIMING) {
        powerup_timer = SDL_GetTicks();

        if (powerup > 0) {
            powerup--;
            energy += static_cast<int>(ceil(static_cast<float>(energy_max) * .1f));
        }

        energy += energy_reg;

        if (life < life_max && energy > life_reg_energy) {
            life += life_reg;
            energy -= life_reg_energy;
        }
    }

    if (life > life_max) {
        life = life_max;
    }

    if (energy > energy_max) {
        energy = energy_max;
    }
}

void Player::draw() {
    float a = state.global_alpha;

    glm::mat4 projection = state.view->getProjection();
    glm::mat4 camera = state.view->getCamera();
    glm::mat4 model = state.view->getModel(
        (getPosX() - state.cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - state.cam_y) * E_RELATIVE_MOVEMENT,
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

        state.shaders[S_TEXTURE]->bind();
        state.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.5f * a, 1.0f * a, .7f * a, gun_flash[0] * a));
        state.shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);

        glDepthMask(GL_FALSE);
        state.textures[T_GLOW]->bind();
        state.textures[T_GLOW]->draw();
        glDepthMask(GL_TRUE);

        state.shaders[S_TEXTURE]->unbind();
    }

    if (gun_flash[1] > 0) {
        glm::mat4 m;

        m = glm::translate(model, glm::vec3(0.5f, 1.25f, 0.5f));
        m = glm::rotate(m, glm::radians(90.0f), glm::vec3(0, 1.0f, 0));
        m = glm::rotate(m, glm::radians(gun_flash_rot[1]), glm::vec3(0, 0, 1.0f));
        m = glm::scale(m, glm::vec3(2.5f, 2.5f, 0));

        state.shaders[S_TEXTURE]->bind();
        state.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.5f * a, 1.0f * a, .7f * a, gun_flash[1] * a));
        state.shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);

        glDepthMask(GL_FALSE);
        state.textures[T_GLOW]->bind();
        state.textures[T_GLOW]->draw();
        glDepthMask(GL_TRUE);

        state.shaders[S_TEXTURE]->unbind();
    }

    state.models[e_obj]->draw(model, camera, projection, glm::vec4(c_r * a, c_g * a, c_b * a, 1.0f));
}
