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

#include "missile.hpp"

Missile::Missile(State* s, uint16_t p) : Entity(s) {
    e_obj = OBJ_MISSILE_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_ACTIVE;

    setScale(2.25f, 2.25f, 2.25f);
    setRot(90.0f, 0, 0);

    c_r = 0.5f;
    c_g = 1.0f;
    c_b = 0.8f;
    c_a = 1.0f;

    v_x = 0;
    v_y = 0;
    v_z = -(E_BASE_SPEED + 100.0f);

    power = p;
}

Missile::~Missile() {
}

void Missile::collide(shared_ptr<Entity> e) {
    if (e->isPlayer()) {
        return;
    }

    if (e->damage(power)) {
        state->audio.playSample(SFX_GUN_IMPACT, 192, 180);
        state->spawn(make_shared<Explosion>(state, OBJ_EXPLOSION_1, getPosX(), getPosY(), getPosZ()));

        e_state = E_STATE_GONE;
    }
}

void Missile::update() {
    Entity::update();

    c_a = (state->global_alpha * .5f) + ((getPosZ() + 200.0f) * .00002f);

    if (c_a < 0 || getPosZ() < -10000.0f) {
        e_state = E_STATE_GONE;
    }
}

void Missile::draw() {
    float a = state->global_alpha;

    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    state->shaders[S_TEXTURE]->bind();
    state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(c_r * a, c_g * a, c_b * a, c_a * a));

    state->textures[T_MISSILE_1]->bind();

    for (int i = 0; i < 5; i++) {
        state->shaders[S_TEXTURE]->update(UNI_MVP, state->view->transform(
            (getPosX() - state->cam_x) * E_RELATIVE_MOVEMENT,
            (getPosY() - state->cam_y) * E_RELATIVE_MOVEMENT,
            getPosZ(),

            getRotX(),
            getRotY() + static_cast<float>(i) * 30.0f,
            getRotZ(),

            20.0f,
            250.0f,
            0
        ));

        state->textures[T_MISSILE_1]->draw();
    }

    state->shaders[S_TEXTURE]->unbind();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}
