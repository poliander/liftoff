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

#include "asteroid.hpp"

Asteroid::Asteroid(State* s) : Entity(s) {
    e_obj = OBJ_ASTEROID_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_IDLE;
}

Asteroid::~Asteroid() {
}

bool Asteroid::damage(int p) {
    bool damaged = Entity::damage(p);

    if (damaged && !isAlive()) {
        state->audio.playSample(SFX_EXPLOSION_2, 192, 180);

        state->spawn(make_shared<Explosion>(state, OBJ_EXPLOSION_2, p_x, p_y, p_z));
        state->spawn(make_shared<Explosion>(state, OBJ_EXPLOSION_3, p_x, p_y, p_z + 5.0f));
        state->spawn(make_shared<Explosion>(state, OBJ_EXPLOSION_4, p_x, p_y, p_z + 10.0f));

        for (int i = 0; i < 4 + rand() % 4; i++) {
            auto debris = make_shared<Debris>(state);

            debris->activate();
            debris->setPos(p_x, p_y, p_z);
            debris->setVelocity(
                .1f * static_cast<float>(rand() % 80) - 4.0f,
                .1f * static_cast<float>(rand() % 80) - 4.0f,
                .1f * static_cast<float>(rand() % 80) - 4.0f
            );

            state->spawn(debris);
        }

        state->notify(MSG_MONEY, static_cast<int16_t>(getScale() * .5f));
    }

    return damaged;
}

void Asteroid::update() {
    Entity::update();

    if (p_z > 0) {
        e_state = E_STATE_GONE;
    }
}
