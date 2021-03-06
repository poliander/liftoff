/*
 *  LIFT-OFF: BEYOND GLAXIUM
 *  Copyright (C) 2019 René Pollesch <poliander@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "player.hpp"

Player::Player(State* s) : Entity(s) {
    e_obj = OBJ_PLAYER;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_ACTIVE;

    init();
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

    agility = 0.5f;
    inertness = 0.1f;

    powerup = 0;
    money = 0;

    gun_power = 30;
    gun_energy = 15;

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

    if (energy < gun_energy) {
        // low energy
        return;
    }

    energy -= gun_energy;
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

    state->spawn(missile);

    angle = static_cast<int>(.5f * (p_x - state->cam_x));

    if (angle < 0) {
        angle += 360;
    }

    state->audio.playSample(2, 255, angle);
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
    if (fabs(a_x) > agility) {
        if (a_x > 0) {
            a_x = agility;
        } else {
            a_x = -agility;
        }
    }

    // limit vertical acceleration
    if (fabs(a_y) > agility) {
        if (a_y > 0) {
            a_y = agility;
        } else {
            a_y = -agility;
        }
    }

    // accelerate horizontally
    if ((a_x - v_x) > .0005f) {
        v_x += (a_x - v_x) * state->global_timer * agility * inertness;
    } else if ((v_x - a_x) > .0005f) {
        v_x -= (v_x - a_x) * state->global_timer * agility * inertness;
    }

    // move horizontally
    p_x -= v_x * state->global_timer * 10.0f;

    // accelerate vertically
    if ((a_y - v_y) > .0005f) {
        v_y += (a_y - v_y) * state->global_timer * agility * inertness;
    } else if ((v_y - a_y) > .0005f) {
        v_y -= (v_y - a_y) * state->global_timer * agility * inertness;
    }

    // move vertically
    p_y -= v_y * state->global_timer * 10.0f;

    a_x = 0;
    a_y = 0;

    // accelerate and move forward/backward
    if (v_z < a_z) {
        v_z += .04f * ((a_z - v_z) + .01f) * state->global_timer;
    } else if (v_z > a_z) {
        v_z -= .04f * ((v_z - a_z) + .01f) * state->global_timer;
    }

    p_z -= v_z * state->global_timer;

    // rotate
    r_x += state->global_timer * w_x * .1f;
    if (r_x < 0) r_x += 360.0f;
    if (r_x > 360.0f) r_x -= 360.0f;

    r_y += state->global_timer * w_y * .1f;
    if (r_y < 0) r_y += 360.0f;
    if (r_y > 360.0f) r_y -= 360.0f;

    r_z += state->global_timer * w_z * .1f;
    if (r_z < 0) r_z += 360.0f;
    if (r_z > 360.0f) r_z -= 360.0f;

    // gun flash animation
    if (gun_flash[0] > 0) gun_flash[0] -= state->global_timer * .15f;
    if (gun_flash[1] > 0) gun_flash[1] -= state->global_timer * .15f;

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
    float a = state->global_alpha;

    glm::mat4 projection = state->view->getProjection();
    glm::mat4 camera = state->view->getCamera();
    glm::mat4 model = state->view->getModel(
        (getPosX() - state->cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - state->cam_y) * E_RELATIVE_MOVEMENT,
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

        state->shaders[S_TEXTURE]->bind();
        state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.5f * a, 1.0f * a, .7f * a, gun_flash[0] * a));
        state->shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);

        glDepthMask(GL_FALSE);
        state->textures[T_GLOW]->bind();
        state->textures[T_GLOW]->draw();
        glDepthMask(GL_TRUE);

        state->shaders[S_TEXTURE]->unbind();
    }

    if (gun_flash[1] > 0) {
        glm::mat4 m;

        m = glm::translate(model, glm::vec3(0.5f, 1.25f, 0.5f));
        m = glm::rotate(m, glm::radians(90.0f), glm::vec3(0, 1.0f, 0));
        m = glm::rotate(m, glm::radians(gun_flash_rot[1]), glm::vec3(0, 0, 1.0f));
        m = glm::scale(m, glm::vec3(2.5f, 2.5f, 0));

        state->shaders[S_TEXTURE]->bind();
        state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.5f * a, 1.0f * a, .7f * a, gun_flash[1] * a));
        state->shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);

        glDepthMask(GL_FALSE);
        state->textures[T_GLOW]->bind();
        state->textures[T_GLOW]->draw();
        glDepthMask(GL_TRUE);

        state->shaders[S_TEXTURE]->unbind();
    }

    state->models[e_obj]->draw(model, camera, projection, glm::vec4(c_r * a, c_g * a, c_b * a, 1.0f));
}
