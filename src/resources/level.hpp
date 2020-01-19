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

#include <string>

#include "../state.hpp"

#include "../entities/asteroid.hpp"
#include "../entities/cargo.hpp"
#include "../entities/debris.hpp"
#include "../entities/entity.hpp"
#include "../entities/player.hpp"
#include "../entities/powerup.hpp"

using std::string;

class Level {
    public:
        explicit Level(State* s) : state(s) {}
        ~Level() {}

        bool load(string filename);

    private:
        State* state;
};
