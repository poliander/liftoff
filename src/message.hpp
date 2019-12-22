#pragma once

#include <stdio.h>

enum message_types_e {
    MSG_MONEY,
    MSG_DAMAGE,
    MSG_ENERGY
};

class Message
{
    public:
        Message(short t, short v);
        virtual ~Message();

        char  text[64];
        float counter;

        short dir_x;
        short dir_y;

        float c_r;
        float c_g;
        float c_b;
};
