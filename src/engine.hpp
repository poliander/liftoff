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
#include <memory>

#include "input.hpp"
#include "scene.hpp"
#include "state.hpp"

class Engine {
    public:
        Engine() {}
        ~Engine() {}

        bool                     init(int argc, char** argv);
        bool                     main();
        void                     halt();

    private:
        State                    state;

        SDL_Window*              window;
        SDL_GLContext            context;

        double                   frame_next;

        unique_ptr<Input>        input;
        unique_ptr<Renderbuffer> buffer;
        unique_ptr<Scene>        scene;

        bool                     initDisplay(int flags);
};
