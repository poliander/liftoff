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
