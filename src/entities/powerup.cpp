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

#include "powerup.hpp"

Powerup::Powerup(State* s, float x, float y, float z) : Entity(s) {
    e_obj = OBJ_POWERUP_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_ACTIVE;

    focusable = true;
    counter = 0;
    life = 1;

    particles = make_unique<ParticleEngine>(s);
    particles->setup(EMIT_EXPLOSION, 15, .5f, .5f, .5f, .05f, 1.0f);
    particles->setSize(200.0f);
    particles->setTexture(T_STAR);
    particles->setColor(.6f, .75f, 1.0f);
    particles->setAlpha(.5f);
    particles->setVolume(10.0f);
    particles->setContinuous(true);

    setPos(x, y, z);
    setScale(50.0f, 50.0f, 50.0f);

    t_r = 0.3f;
    t_g = 0.55f;
    t_b = 1.0f;
}

bool Powerup::damage(int p) {
    return false;
}

void Powerup::collide(shared_ptr<Entity> e) {
    if (e_state == E_STATE_ACTIVE && e->isPlayer()) {
        state->audio.playSample(SFX_POWERUP_1, 192, 180);
        state->notify(MSG_ENERGY, 500);
        e->collect(e_obj);
        e_state = E_STATE_FADING;
        v_z = -.5f * E_BASE_SPEED;
    }
}

void Powerup::update() {
    Entity::update();

    particles->update();

    if (e_state == E_STATE_FADING) {
        counter += state->global_timer * .01f;
        particles->setContinuous(false);
        particles->setIncrease(-10.0f);
    }

    if (getPosZ() > 0 || counter > 5.0f) {
        e_state = E_STATE_GONE;
    }
}

void Powerup::draw() {
    particles->draw(
        (getPosX() - state->cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - state->cam_y) * E_RELATIVE_MOVEMENT,
        (getPosZ()),
        (getRotX()),
        (getRotY()),
        (getRotZ())
    );
}
