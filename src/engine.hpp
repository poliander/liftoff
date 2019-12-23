#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "scene.hpp"
#include "state.hpp"
#include "renderbuffer.hpp"

class Engine
{
    public:
        Engine();
        ~Engine();

        bool                     init(int argc, char **argv);
        bool                     main();
        void                     halt();

    private:
        State                    state;

        SDL_Window*              window;
        SDL_GLContext            context;

        Uint32                   timer;

        unique_ptr<Scene>        scene;
        unique_ptr<Renderbuffer> buffer;

        bool                     initDisplay();

        bool                     loadConfiguration();
        bool                     writeConfiguration();

        bool                     handleKeyboard();
        void                     handleJoystick();
};
