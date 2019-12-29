#pragma once

#include <stdint.h>
#include <stdio.h>

enum message_types_e {
    MSG_MONEY,
    MSG_DAMAGE,
    MSG_ENERGY
};

class Message
{
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
