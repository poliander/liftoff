#include "state.hpp"

State::State()
{
    char *buf;
    long psize = 255;

    if ((buf = (char *)malloc((size_t)psize)) != NULL) {
        getcwd(buf, (size_t)psize);
        sprintf(engine_datadir, "%s/resources", buf);
        delete [] buf;
    }

    config.vid_width        = DEFAULT_VID_WIDTH;
    config.vid_height       = DEFAULT_VID_HEIGHT;
    config.vid_fullscreen   = DEFAULT_VID_FULLSCREEN;
    config.vid_vsync        = DEFAULT_VID_VSYNC;
    config.vid_aspect       = 0;

    config.aud_mixfreq      = DEFAULT_AUD_MIXFREQ;
    config.aud_sfx          = 2;
    config.aud_music        = 2;

    vid_cfg_multisampling   = 8;

    log_file                = false;

    engine_restart          = false;
    engine_stars            = DEFAULT_GFX_STARS;
    engine_stars_warp       = DEFAULT_GFX_STARS_WARP;

    fps                     = 0;
    fps_visible             = false;
    fps_count               = 100;
    fps_ready               = false;
    fps_frame               = 0;

    joystick                = NULL;
    mouse_moved             = true;
    mouse_pressed           = false;
    mouse_released          = true;

    tilt_factor             = .0f;
    tilt_x                  = .0f;
    tilt_y                  = .0f;
    tilt_dx                 = .0f;
    tilt_dy                 = .0f;

    id                      = 0;
}

State::~State()
{
}

/*
 * log output
 */
void State::log(const char *msg)
{
#ifdef _WIN32
    if (log_file) {
        FILE *fp;

        fp = fopen("debug.log", "a");
        fprintf(fp, "%s", msg);
        fclose(fp);
    }
#else
    printf("%s", msg);
#endif
}

/*
 * camera shaking ("tilt")
 */
void State::tilt(float t)
{
    if (tilt_factor < fabs(t)) {
        tilt_factor = fabs(t);
    }
}

/*
 * add money/energy message
 */
void State::addMessage(int value, unsigned short type)
{
    if (msg_num >= E_MAX_MESSAGES-1) return;

    switch(type) {
        case MSG_DAMAGE:
            msg[msg_num].direction_x = -1;
            msg[msg_num].direction_y = -1;
            sprintf(msg[msg_num].text, "-%d", value);
            break;

        case MSG_MONEY:
            msg[msg_num].direction_x = 1;
            msg[msg_num].direction_y = -1;
            sprintf(msg[msg_num].text, "%d $", value);
            break;

        case MSG_ENERGY:
            msg[msg_num].direction_x = -1;
            msg[msg_num].direction_y = -1;
            sprintf(msg[msg_num].text, "+%d", value);
            break;

        default:
            return;
    }

    msg[msg_num].type = type;
    msg[msg_num].counter = .0f;

    msg_num++;
}

/*
 * set or change engine state
 */
bool State::set(int s)
{
    if (id == s) return false;

    char msg[255];
    int x, y;

    sprintf(msg, "Entering state %d\n", s);
    log(msg);

    switch(s) {
        case STATE_MENU:
            timer_adjustment = 0;

            stars_warp = true;
            stars_speed = 1.75f;
            stars_rotation = true;
            stars_rotation_pos = .0f;
            stars_rotation_speed = .05f;

            menu = 1;
            menu_pos = 0;
            menu_selected = false;

            title_ypos = .0f;
            global_alpha = 0;
            engine_restart = false;

            audio.playMusic(0, 1000);
            audio.stopSampleLoop(0);

            SDL_GetMouseState(&x, &y);
            mouse_x = (-3.1f * vid_cfg_aspect) + (1 / float(config.vid_width)) * x * (6.35f * vid_cfg_aspect);
            mouse_y = 3.1f + (-1 / float(config.vid_height)) * y * 6.35f;
            mouse_moved = true;
            break;

        case STATE_QUIT:
            title_ypos = 99.85f;
            audio.stopMusic(1000);
            break;

        case STATE_GAME_START:
            global_alpha = 99;
            title_ypos = 99.85f;
            cam_x = .0f;
            cam_y = .0f;
            hud_x = -20.0f;
            hud_y = -10.0f;
            lvl_pos = 0;
            lvl_loaded = true;
            engine_boundary = true;
            stars_warp = true;
            stars_speed = 1.75f;

            audio.stopMusic(1000);

            if (strlen(lvl_music)) {
                audio.music[1] = audio.loadMusic(lvl_music);
            }
            break;

        case STATE_GAME_LOOP:
            menu = 0;

            stars_warp = false;
            stars_rotation = false;
            stars_speed = .35f;

            cam_x = -250.0f;
            if ((rand() % 2) == 1) cam_x = -cam_x;

            cam_y = -400.0f;
            if ((rand() % 2) == 1) cam_y = -cam_y;

            cam_y_offset = 35.0f;
            cam_speed = .015f;

            audio.playSampleLoop(3, 1000);
            break;

        case STATE_GAME_QUIT:
            title_ypos = 0;
            audio.stopMusic(1000);
            audio.stopSampleLoop(1000);
            break;

        case STATE_GAME_NEXTLEVEL:
            stars_warp = true;
            engine_boundary = false;
            audio.playSample(10, 255, 0);
            audio.stopSampleLoop(1000);
            audio.stopMusic(2500);
            break;
    }

    id = s;

    return true;
}

/*
 * return current state id
 */
int State::get(void)
{
    return id;
}

