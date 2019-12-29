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

#include <stdlib.h>

#include <algorithm>
#include <cmath>
#include <memory>

#include "functions.hpp"
#include "framebuffer.hpp"
#include "state.hpp"

#define SKYBOX_NUM_STARS      3000
#define SKYBOX_NUM_STARS_WARP 500

using std::max;

class Skybox {
    public:
        explicit Skybox(State* s);
        ~Skybox();

        void                    update();
        void                    draw();

    private:
        float                   stars[SKYBOX_NUM_STARS][4];

        State*                  state;

        unique_ptr<View>        view;
        unique_ptr<Framebuffer> framebuffer;
};
