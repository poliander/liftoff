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

using namespace std;

class Entity;
class Font;

class State
{
    public:
        State();
        ~State();

        Audio                       audio;
        Configuration               config;
        View                        view;

        // resources
        map<unsigned int, Texture*> textures;
        map<unsigned int, Shader*>  shaders;
        map<unsigned int, Model*>   models;
        map<unsigned int, Font*>    fonts;

        // scenery
        shared_ptr<Entity>          player;
        vector<shared_ptr<Entity>>  entities;

        // timer
        Uint32                      timer;
        float                       timer_adjustment;

        int                         global_alpha;

        bool                        log_file;
        bool                        cfg_loaded;

        char                        dir_configuration[255];
        char                        dir_resources[255];

        bool                        engine_restart;
        bool                        engine_boundary;

        // video capabilities
        int                         vid_cap_modes_num;
        SDL_DisplayMode             vid_cap_modes[128];

        // hardware configuration
        int                         vid_display;
        int                         vid_mode;
        int                         vid_width;
        int                         vid_height;
        int                         vid_refresh_rate;
        Uint32                      vid_format;
        float                       vid_aspect;
        unsigned short              vid_aspect_mode;
        unsigned short              vid_fullscreen;
        unsigned short              vid_vsync;
        unsigned short              vid_multisampling;

        // money/damage messages
        unsigned short              msg_num = 0;
        Message                     msg[E_MAX_MESSAGES];

        // game menu
        int                         menu;
        int                         menu_pos;
        float                       menu_title_pos;
        bool                        menu_selected;

        // stars
        float                       stars_speed;
        int                         stars_rotation;
        float                       stars_rotation_pos;
        float                       stars_rotation_speed;
        bool                        stars_warp;

        // frames per second
        float                       fps;         // calculated fps rate
        Uint32                      fps_timer;   // measuring timer
        Uint32                      fps_timer_l; // previous frame measuring time
        bool                        fps_visible; // toggle display
        int                         fps_counter; // frames counter

        // input
        SDL_Joystick*               joystick;

        // level
        unsigned short              lvl_id;
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

        // hud position
        float                       hud_x;
        float                       hud_y;

        // add message
        void                        addMessage(int value, unsigned short type);

        // view related
        void                        tilt(float t);

        // state
        bool                        set(int s);
        int                         get();

        void                        log(const char *msg);

    private:
        int                         id;
};
