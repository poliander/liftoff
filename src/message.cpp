#include <message.hpp>

Message::Message(short type, short value)
{
    counter = 0;

    switch (type) {
        case MSG_DAMAGE:
            c_r = 1.0f;
            c_g = 0.4f;
            c_b = 0.2f;

            dir_x = -1;
            dir_y = -1;

            sprintf(text, "-%d", value);
            break;

        case MSG_MONEY:
            c_r = 1.0f;
            c_g = 1.0f;
            c_b = 0.3f;

            dir_x = 1;
            dir_y = -1;

            sprintf(text, "%d $", value);
            break;

        case MSG_ENERGY:
            c_r = 0.3f;
            c_g = 0.8f;
            c_b = 1.0f;

            dir_x = -1;
            dir_y = -1;

            sprintf(text, "+%d", value);
            break;
    }
}

Message::~Message()
{
}
