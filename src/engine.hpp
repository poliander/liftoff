#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "scenery.hpp"

class Engine
{
    public:
        Engine();
        ~Engine();

        bool init(int argc, char **argv);
        bool main();
        void halt();

    private:
        State state;
        Scenery* scenery;

        SDL_Surface* screen;

        bool loadConfiguration();
        bool writeConfiguration();

        bool initDisplay();

        bool handleKeyboard();
        void handleJoystick();
        void handleMouse();
};

#endif
