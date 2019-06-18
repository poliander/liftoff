#pragma once

struct message_t
{
    short   type;
    char    text[64];

    float   counter;
    short   direction_x;
    short   direction_y;
};
