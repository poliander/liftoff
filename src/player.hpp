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

#include <cmath>
#include <memory>

#include <glm/glm.hpp>

#include "entity.hpp"
#include "missile.hpp"

class Player : public Entity {
    public:
        explicit Player(State* s);
        ~Player();

        void            init();
        void            update();
        void            draw();
        void            shoot();
        void            collide(shared_ptr<Entity> e);
        void            collect(uint16_t e_obj);

    private:
        uint8_t         m_alt;
        Uint32          m_next_shot_timer;
        Uint32          powerup_timer;

        float           gun_flash[2];
        float           gun_flash_rot[2];
};
