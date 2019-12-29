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

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>

#include "definitions.hpp"

struct config_t {
    uint16_t        vid_width;
    uint16_t        vid_height;

    uint8_t         vid_quality;
    uint8_t         vid_fullscreen;
    uint8_t         vid_vsync;

    uint8_t         aud_sfx;
    uint8_t         aud_music;
    uint16_t        aud_mixfreq;
};

class Configuration {
    public:
        static bool load(const char* dir, config_t* c);
        static bool save(const char* dir, config_t* c);
};
