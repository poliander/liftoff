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

#include "debris.hpp"

Debris::Debris(State* s) : Entity(s) {
    e_obj = OBJ_DEBRIS_1;
    e_type = E_TYPE_DECORATION;
    e_state = E_STATE_IDLE;

    p_x = -600.0f + static_cast<float>(rand() % 1200);
    p_y = -400.0f + static_cast<float>(rand() % 800);
    p_z = -8000.0f;

    w_x = static_cast<float>(rand() % 100) * .1f;
    w_y = static_cast<float>(rand() % 100) * .1f;
    w_z = static_cast<float>(rand() % 100) * .1f;

    s_x = 20.0f + static_cast<float>(rand() % 400) * .2f;
    s_y = 20.0f + static_cast<float>(rand() % 400) * .2f;
    s_z = 20.0f + static_cast<float>(rand() % 400) * .2f;

    c_r = .6f;
    c_g = .6f;
    c_b = .6f;
}

Debris::~Debris() {
}

void Debris::update() {
    Entity::update();

    if (p_z > 0) {
        e_state = E_STATE_GONE;
    }
}
