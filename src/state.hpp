#ifndef STATE_HPP_
#define STATE_HPP_

using namespace std;

#include <map>
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

// object
struct object_t
{
    int                 type;       // OBJ_TYPE_*
    int                 state;      // OBJ_STATE_*
    int                 id;         // OBJ_*

    int                 life;
    int                 life_max;
    int                 life_time;
    int                 target;

    int                 money;
    int                 energy;
    unsigned short      powerup;

    float               cnt;
    float               speed;

    float               pos_x;      // position
    float               pos_y;
    float               pos_z;

    float               s_x;        // speed
    float               s_y;
    float               s_z;

    float               a_x;        // acceleration
    float               a_y;
    float               a_z;

    float               rot_x;      // rotation
    float               rot_y;
    float               rot_z;

    float               rsp_x;      // rotation speed
    float               rsp_y;
    float               rsp_z;

    float               scale_x;    // object scaling
    float               scale_y;
    float               scale_z;
};

class Entity;

class State
{
    public:
        State();
        ~State();

        // audio subsystem and resources
        Audio                       audio;

        // game resources
        config_t                    config;
        object_t                    objects[E_MAX_OBJECTS];
        unsigned int                texture[20];

        vector<shared_ptr<Entity>>  entities;
        map<unsigned int, Model*>   models;

        // timer
        Uint32          timer;
        float           timer_adjustment;

        int             global_alpha;
        float           title_ypos;

        int             player;

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
        unsigned short  msg_num;
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

        // entities and money/damage messages
        bool            add(object_t *n_obj);
        void            addMessage(int value, unsigned short type);
        void            sort();
        void            remove(int oid);
        void            explode(int oid);

        // state
        bool            set(int s);
        int             get();

        void            log(const char *msg);

    private:
        int             id;

        bool            objectLoad(int id, const char *filename);
        bool            objectLoadFirstPass(int id, FILE *fp);
        bool            objectLoadSecondPass(int id, FILE *fp);
        bool            objectMemAlloc(int id);
        void            objectFreemem(int id);
};

#endif
