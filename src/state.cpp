#include "state.hpp"

State::State()
{
    char cwd[255];

    getcwd(cwd, 255);

    if (chdir(DEFAULT_GAMEDATA) == 0) {
        sprintf(dir_resources, "%s", DEFAULT_GAMEDATA);
    } else {
        sprintf(dir_resources, "%s", GAMEDATA);
    }

    chdir(cwd);

#ifdef _WIN32
    sprintf(dir_configuration, "%s\\LiftOff", getenv("APPDATA"));
#else
    sprintf(dir_configuration, "%s/.liftoff", getenv("HOME"));
#endif

    config.vid_width        = DEFAULT_VID_WIDTH;
    config.vid_height       = DEFAULT_VID_HEIGHT;
    config.vid_fullscreen   = DEFAULT_VID_FULLSCREEN;
    config.vid_vsync        = DEFAULT_VID_VSYNC;
    config.vid_quality      = 0;

    config.aud_mixfreq      = DEFAULT_AUD_MIXFREQ;
    config.aud_sfx          = 2;
    config.aud_music        = 2;

    vid_multisampling       = DEFAULT_VID_MSAA;
    vid_width               = DEFAULT_VID_WIDTH;
    vid_height              = DEFAULT_VID_HEIGHT;
    vid_fullscreen          = DEFAULT_VID_FULLSCREEN;
    vid_vsync               = DEFAULT_VID_VSYNC;
    vid_aspect              = 0;
    vid_display             = -1;

    log_file                = false;
    cfg_loaded              = false;
    engine_restart          = false;

    fps_visible             = false;
    fps_counter             = 0;
    fps_timer               = 0;
    fps_timer_l             = 0;

    joystick                = NULL;

    tilt_factor             = 0;
    tilt_x                  = 0;
    tilt_y                  = 0;
    tilt_dx                 = 0;
    tilt_dy                 = 0;

    id                      = 0;

    timer                   = SDL_GetTicks();

    entities.clear();
    spawns.clear();
    messages.clear();
}

State::~State()
{
}

void State::log(const char *msg)
{
    if (log_file) {
        FILE *fp;

        fp = fopen("liftoff.log", "a");
        fprintf(fp, "%s", msg);
        fclose(fp);
    } else {
        printf("%s", msg);
    }
}

void State::tilt(float t)
{
    if (tilt_factor < fabs(t)) {
        tilt_factor = fabs(t);
    }
}

void State::spawn(shared_ptr<Entity> e)
{
    spawns.push_back(e);
}

void State::notify(uint8_t t, int16_t v)
{
    messages.push_back(make_unique<Message>(t, v));
}

void State::update()
{
    global_timer = static_cast<float>(SDL_GetTicks() - timer) * .05f;

    if (global_counter < 1.0f) {
        global_counter += global_timer * .02f;
        global_transition1 = 0.5f + 0.5f * cos(M_PI + global_counter * M_PI);
    } else {
        global_counter = 1.0f;
        global_transition1 = 1.0f;
    }

    if (global_counter2 < 1.0f) {
        global_counter2 += global_timer * .01f;
        global_transition2 = -pow(-global_counter2 + 1.0f, 3) + 1.0f;
    } else {
        global_counter2 = 1.0f;
        global_transition2 = 1.0f;
    }

    // frames per second

    if (fps_timer_l > 0) {
        fps_timer += timer - fps_timer_l;

        if (fps_timer > 1000) {
            fps = static_cast<float>(fps_counter) / (static_cast<float>(fps_timer) * .001f);
            fps_counter = 0;
            fps_timer = 0;
        }
    }

    fps_counter++;
    fps_timer_l = timer;

    timer = SDL_GetTicks();
}

bool State::set(int s)
{
    if (id == s) return false;

    char msg[255];
    sprintf(msg, "Entering state %d\n", s);
    log(msg);

    global_counter = 0;
    global_counter2 = 0;
    global_transition1 = 0;
    global_transition2 = 0;

    switch (s) {
        case STATE_MENU:
            timer = SDL_GetTicks();

            global_alpha = 0;
            global_timer = 0;

            stars_warp = true;
            stars_speed = 1.75f;
            stars_rotation_speed = .05f;

            menu = 1;
            menu_pos = 0;
            menu_selected = false;

            cam_x = 0;
            cam_y = 0;

            engine_restart = false;

            audio.playMusic(0, 1000);
            audio.stopSampleLoop(0);
            break;

        case STATE_GAME_START:
            global_alpha = 1.0f;

            lvl_pos = 0;
            lvl_loaded = false;

            audio.stopMusic(1000);

            if (strlen(lvl_music)) {
                audio.music[1] = audio.loadMusic(lvl_music);
            }
            break;

        case STATE_GAME_LOOP:
            menu = 0;

            stars_warp = false;
            stars_speed = .35f;
            stars_rotation_speed = 0;

            cam_x = -250.0f;
            if ((rand() % 2) == 1) cam_x = -cam_x;

            cam_y = -400.0f;
            if ((rand() % 2) == 1) cam_y = -cam_y;

            cam_y_offset = 35.0f;
            cam_speed = .015f;

            audio.playSampleLoop(3, 1000);
            break;

        case STATE_GAME_QUIT:
            audio.stopMusic(1000);
            audio.stopSampleLoop(1000);
            break;

        case STATE_GAME_NEXTLEVEL:
            stars_warp = true;
            audio.playSample(10, 255, 0);
            audio.stopSampleLoop(1000);
            audio.stopMusic(2500);
            break;

        case STATE_QUIT:
            audio.stopMusic(1000);
            break;
    }

    id = s;

    return true;
}

int State::get()
{
    return id;
}
