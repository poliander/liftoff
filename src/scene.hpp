/*
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

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <stdlib.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../config.h"

#include "functions.hpp"
#include "state.hpp"
#include "entity.hpp"
#include "asteroid.hpp"
#include "cargo.hpp"
#include "debris.hpp"
#include "explosion.hpp"
#include "overlay.hpp"
#include "player.hpp"
#include "powerup.hpp"
#include "renderbuffer.hpp"
#include "skybox.hpp"

using std::unique_ptr;

class Scene {
    public:
        explicit Scene(State* s);
        ~Scene();

        void                load();
        void                update();
        void                draw(const unique_ptr<Renderbuffer> &b);

    private:
        State*              state;

        shared_ptr<Player>  player;
        unique_ptr<Skybox>  skybox;
        unique_ptr<Overlay> overlay;

        void                updateScene();
        bool                loadLevel();
};
