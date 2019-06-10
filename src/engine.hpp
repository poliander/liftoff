#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "scenery.hpp"

class Engine {
    public:
        Engine();
        ~Engine();

        bool init(int argc, char **argv);
        void shutdown();
        bool main();

    protected:
        State state;
        Scenery* scenery;

        SDL_Surface* screen;

        bool loadConfiguration();
        bool writeConfiguration();

        void getVideoModeID();
        bool initDisplay();

        // input handlers
        bool keyHandler();
        void joyHandler();
        void mouseHandler();
};

extern Engine* engine;

#endif /* ENGINE_HPP_ */
