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

#include <message.hpp>

Message::Message(uint16_t t, uint16_t v) {
    counter = 0;
    type = t;
    value = v;

    switch (type) {
        case MSG_DAMAGE:
            c_r = 1.0f;
            c_g = 0.4f;
            c_b = 0.2f;

            dir_x = +1;
            dir_y = +1;

            snprintf(text, sizeof(text), "-%d", value);
            break;

        case MSG_MONEY:
            c_r = 1.0f;
            c_g = 1.0f;
            c_b = 0.3f;

            dir_x = -1;
            dir_y = +1;

            snprintf(text, sizeof(text), "%d $", value);
            break;

        case MSG_ENERGY:
            c_r = 0.3f;
            c_g = 0.8f;
            c_b = 1.0f;

            dir_x = +1;
            dir_y = +1;

            snprintf(text, sizeof(text), "+%d", value);
            break;
    }
}

Message::~Message() {
}
