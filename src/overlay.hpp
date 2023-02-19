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

#include <memory>

#include "renderbuffer.hpp"
#include "state.hpp"
#include "entities/player.hpp"

using std::make_pair;
using std::unique_ptr;

class Overlay {
    public:
        explicit Overlay(State* s);
        ~Overlay() {}

        void update();
        void draw();

    private:
        State*                   state;

        unique_ptr<Renderbuffer> buffer;
        unique_ptr<View>         view;
        unique_ptr<View>         perspective;

        void                     drawScreen();
        void                     drawShip();
        void                     drawMenu();
        void                     drawStatus();
        void                     drawMessages();

        float                    screen_y;
        float                    ship_y;

        float                    logo1_x;
        float                    logo1_y;
        float                    logo2_x;
        float                    logo2_y;

        bool                     menu_visible;
        bool                     menu_cursor_visible;
        float                    menu_alpha;

        bool                     status_visible;
        float                    status_alpha;
};
