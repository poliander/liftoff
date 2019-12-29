/*
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

#include "cargo.hpp"

Cargo::Cargo(State* s) : Entity(s) {
    e_obj = OBJ_CARGO_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_IDLE;

    counter = .01f;
    focusable = true;
    money = 50;
}

Cargo::~Cargo() {
}

bool Cargo::damage(int p) {
    bool damaged = Entity::damage(p);

    if (damaged && e_state == E_STATE_GONE && yield == false) {
        state->audio.playSample(SFX_EXPLOSION_1, 192, 180);

        state->spawn(make_shared<Explosion>(state, OBJ_EXPLOSION_3, p_x, p_y, p_z, .6f, .75f, 1.0f));
        state->spawn(make_shared<Powerup>(state, p_x, p_y, p_z));

        state->notify(MSG_MONEY, money);
        e_state = E_STATE_FADING;
        yield = true;
    }

    return damaged;
}

void Cargo::update() {
    Entity::update();

    if (e_state == E_STATE_FADING) {
        counter += state->global_timer * .2f;
    }

    if (p_z > 0 || counter > 5.0f) {
        e_state = E_STATE_GONE;
    }
}

void Cargo::draw() {
    float a = state->global_alpha, d = calcDistanceScale();

    glm::vec4 color = glm::vec4(a, a, a, a);
    glm::mat4 projection = state->view->getProjection();
    glm::mat4 camera = state->view->getCamera();
    glm::mat4 m, model = state->view->getModel(
        (getPosX() - state->cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - state->cam_y) * E_RELATIVE_MOVEMENT,
        getPosZ(),

        getRotX(),
        getRotY(),
        getRotZ(),

        getScaleX() * d,
        getScaleY() * d,
        getScaleZ() * d
    );

    state->shaders[S_TEXTURE]->bind();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    if (e_state == E_STATE_FADING) {
        float scale = -1.0f / (.2f * counter + .2f) + 4.5f;

        m = state->view->getModel(
            (getPosX() - state->cam_x) * E_RELATIVE_MOVEMENT,
            (getPosY() - state->cam_y) * E_RELATIVE_MOVEMENT,
            (getPosZ()),

            0, 0, 0,

            d * scale * 150.0f,
            d * scale * 150.0f,
            0
        );

        state->textures[T_HALO]->bind();
        state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.8f * a, .9f * a, 1.0f * a, a * (1.0f - (counter * .2f))));
        state->shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);
        state->textures[T_HALO]->draw();
    } else {
        state->textures[T_GLOW]->bind();

        m = model;
        m = glm::rotate(m, glm::radians(90.0f), glm::vec3(1.0f, 0, 0));
        m = glm::scale(m, glm::vec3(15.0f, 15.0f, 0));

        state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.8f * a, .9f * a, 1.0f * a, 0.45f * a));
        state->shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);
        state->textures[T_GLOW]->draw();

        m = model;
        m = glm::rotate(m, glm::radians(90.0f), glm::vec3(0, 1.0f, 0));
        m = glm::scale(m, glm::vec3(15.0f, 15.0f, 0));

        state->shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);
        state->textures[T_GLOW]->draw();

        m = model;
        m = glm::rotate(m, glm::radians(90.0f), glm::vec3(0, 0, 1.0f));
        m = glm::scale(m, glm::vec3(15.0f, 15.0f, 0));

        state->shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);
        state->textures[T_GLOW]->draw();
    }

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    state->shaders[S_TEXTURE]->unbind();

    if (e_state == E_STATE_ACTIVE) {
        state->models[e_obj]->draw(model, camera, projection, color);
    }
}
