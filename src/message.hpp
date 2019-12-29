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

#pragma once

#include <stdio.h>

#include <cstdint>

enum message_types_e {
    MSG_MONEY,
    MSG_DAMAGE,
    MSG_ENERGY
};

class Message {
    public:
        Message(uint16_t t, uint16_t v);
        virtual ~Message();

        char     text[64];
        uint16_t type;
        uint16_t value;
        float    counter;

        int8_t dir_x;
        int8_t dir_y;

        float c_r;
        float c_g;
        float c_b;
};
