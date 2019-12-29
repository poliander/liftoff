#pragma once

#include <map>
#include <cmath>
#include <memory>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include "../config.h"

#include "audio.hpp"
#include "configuration.hpp"
#include "definitions.hpp"
#include "font.hpp"
#include "model.hpp"
#include "message.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "view.hpp"

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

class Entity;

class State {
    public:
        State();
        ~State();

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
        float                       global_counter;
        float                       global_counter2;
        float                       global_transition1;
        float                       global_transition2;

        bool                        log_file;
        bool                        cfg_loaded;
        bool                        engine_restart;
        Uint32                      timer;

        char                        dir_configuration[255];
        char                        dir_resources[255];

        // display
        SDL_DisplayMode             vid_cap_modes[128];
        int                         vid_cap_modes_num;
        int                         vid_mode;
        int                         vid_display;

        uint16_t                    vid_width;
        uint16_t                    vid_height;
        uint8_t                     vid_quality;
        uint8_t                     vid_fullscreen;
        uint8_t                     vid_vsync;

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

        // camera tilt
        float                       tilt_x;
        float                       tilt_y;
        float                       tilt_dx;
        float                       tilt_dy;
        float                       tilt_factor;

        // state
        bool                        set(int s);
        int                         get();
        void                        update();

        void                        tilt(float t);
        void                        spawn(shared_ptr<Entity> e);
        void                        notify(uint8_t t, int16_t v);
        void                        log(const char* msg);

    private:
        int                         id;
};
