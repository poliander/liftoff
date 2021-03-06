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

#include <SDL2/SDL.h>

#include <map>
#include <cmath>
#include <ctime>
#include <memory>
#include <vector>

#include "../config.h"

#include "audio.hpp"
#include "configuration.hpp"
#include "definitions.hpp"
#include "model.hpp"
#include "message.hpp"
#include "view.hpp"
#include "resources/font.hpp"
#include "resources/shader.hpp"
#include "resources/texture.hpp"

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

class Entity;

class State {
    public:
        State();
        ~State() {};

        Audio                       audio;
        config_t                    config;

        // resources
        map<unsigned int, Texture*> textures;
        map<unsigned int, Shader*>  shaders;
        map<unsigned int, Model*>   models;
        map<unsigned int, Font*>    fonts;

        // scenery
        unique_ptr<View>            view;
        shared_ptr<Entity>          player;
        vector<shared_ptr<Entity>>  entities;
        vector<shared_ptr<Entity>>  spawns;
        vector<shared_ptr<Message>> messages;

        // globals
        float                       global_timer;
        float                       global_alpha;
        float                       global_transition;
        float                       global_transition_speed;

        bool                        log_file;
        Uint32                      timer;

        char                        dir_configuration[255];
        char                        dir_resources[255];

        // display
        map<int, SDL_DisplayMode>   vid_modes;
        uint16_t                    vid_width;
        uint16_t                    vid_height;
        uint8_t                     vid_quality;
        uint8_t                     vid_fullscreen;
        uint8_t                     vid_vsync;
        int                         vid_mode;
        int                         vid_display;

        int                         vid_refresh_rate;
        int                         vid_format;
        float                       vid_aspect;
        uint8_t                     vid_multisampling;
        uint16_t                    vid_fb_size;
        uint8_t                     vid_font_resolution;

        // game menu
        int                         menu;
        int                         menu_pos;
        bool                        menu_selected;

        // stars
        float                       stars_speed;
        float                       stars_rotation_pos;
        float                       stars_rotation_speed;
        bool                        stars_warp;

        // frames per second
        float                       fps;          // actual FPS rate
        bool                        fps_visible;  // toggle display
        int                         fps_counter;  // frames counter
        Uint32                      fps_timer;    // measuring timer
        Uint32                      fps_timer_l;  // previous frame measuring time

        // input
        SDL_Joystick*               joystick;

        // level
        bool                        lvl_loaded;
        char                        lvl_music[32];
        float                       lvl_pos;
        int                         lvl_length;
        int                         lvl_entities;

        // chase cam
        float                       cam_x;
        float                       cam_y;
        float                       cam_y_offset;
        float                       cam_speed;

        // state
        bool                        set(int s);
        int                         get();
        void                        update();

        void                        spawn(shared_ptr<Entity> e);
        void                        notify(uint8_t t, int16_t v);
        void                        log(const char* msg);

    private:
        int                         id;
};
