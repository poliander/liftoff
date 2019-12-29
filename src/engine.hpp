#pragma once

#include <cstdlib>
#include <ctime>
#include <memory>

#include "scene.hpp"
#include "state.hpp"
#include "renderbuffer.hpp"

class Engine {
    public:
        Engine();
        ~Engine();

        bool                     init(int argc, char** argv);
        bool                     main();
        void                     halt();

    private:
        State                    state;

        SDL_Window*              window;
        SDL_GLContext            context;

        unique_ptr<Scene>        scene;
        unique_ptr<Renderbuffer> buffer;

        bool                     initDisplay();

        bool                     handleKeyboard();
        void                     handleJoystick();
};
