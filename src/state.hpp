#ifndef STATE_HPP_
#define STATE_HPP_

using namespace std;

#include <map>
#include <math.h>
#include <memory>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>

#include <SDL.h>

#include "audio.hpp"
#include "config.hpp"
#include "definitions.hpp"
#include "message.hpp"
#include "model.hpp"
#include "texture.hpp"

class Entity;

class State
{
    public:
        State();
        ~State();

        // audio subsystem
        Audio                       audio;

        // game resources
        config_t                    config;

        map<unsigned int, Texture*> textures;
        map<unsigned int, Model*>   models;

        shared_ptr<Entity>          player;
        vector<shared_ptr<Entity>>  entities;

        // timer
        Uint32          timer;
        float           timer_adjustment;

        int             global_alpha;
        float           title_ypos;

        bool            log_file;

        int             engine_stars;
        int             engine_stars_warp;

        bool            engine_restart;
        bool            engine_boundary;
        bool            engine_debug;
        char            engine_cfgfile[255];
        char            engine_datadir[255];

        // hardware capabilities
        bool            vid_sup_fullscreen;
        bool            vid_sup_window;
        int             vid_sup_modes_count;
        SDL_Rect**      vid_sup_modes;

        // hardware configuration
        int             vid_cfg_mode;
        int             vid_cfg_width;
        int             vid_cfg_height;
        float           vid_cfg_aspect;
        unsigned short  vid_cfg_aspect_mode;
        unsigned short  vid_cfg_fullscreen;
        unsigned short  vid_cfg_vsync;
        int             vid_cfg_depth;
        unsigned short  vid_cfg_multisampling;

        // money/damage messages
        unsigned short  msg_num = 0;
        message_t       msg[E_MAX_MESSAGES];

        // game menu
        int             menu;
        int             menu_pos;
        bool            menu_selected;

        // background stars
        float           stars_speed;
        int             stars_rotation;
        float           stars_rotation_pos;
        float           stars_rotation_speed;
        bool            stars_warp;

        // frames per second
        float           fps;
        bool            fps_visible;
        bool            fps_ready;
        int             fps_count;
        int             fps_frame;
        int             fps_dt;
        int             fps_lt;

        // input
        float           mouse_x;
        float           mouse_y;
        bool            mouse_moved;
        bool            mouse_pressed;
        bool            mouse_released;
        Uint8           mouse_button;
        SDL_Joystick*   joystick;

        // level
        unsigned short  lvl_id = 1;
        bool            lvl_loaded;
        char            lvl_music[32];
        float           lvl_pos;
        int             lvl_length;
        int             lvl_entities;

        // chase cam
        float           cam_x;
        float           cam_y;
        float           cam_y_offset;
        float           cam_speed;

        // camera tilt
        float           tilt_x;
        float           tilt_y;
        float           tilt_dx;
        float           tilt_dy;
        float           tilt_factor;

        // hud position
        float           hud_x;
        float           hud_y;

        // add message
        void            addMessage(int value, unsigned short type);

        // view related
        void            tilt(float t);

        // state
        bool            set(int s);
        int             get();

        void            log(const char *msg);

    private:
        int             id;
};

#endif
