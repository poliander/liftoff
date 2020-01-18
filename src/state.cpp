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

#include "state.hpp"

State::State() {
    char cwd[255];

    getcwd(cwd, 255);

    if (chdir(DEFAULT_GAMEDATA) == 0) {
        snprintf(dir_resources, sizeof(dir_resources), "%s", DEFAULT_GAMEDATA);
    } else {
        snprintf(dir_resources, sizeof(dir_resources), "%s", GAMEDATA);
    }

    chdir(cwd);

#ifdef _WIN32
    snprintf(dir_configuration, sizeof(dir_configuration), "%s\\LiftOff", getenv("APPDATA"));
#else
    snprintf(dir_configuration, sizeof(dir_configuration), "%s/.liftoff", getenv("HOME"));
#endif

    srand((int)time(NULL));

    config.vid_width        = DEFAULT_VID_WIDTH;
    config.vid_height       = DEFAULT_VID_HEIGHT;
    config.vid_fullscreen   = DEFAULT_VID_FULLSCREEN;
    config.vid_vsync        = DEFAULT_VID_VSYNC;
    config.vid_quality      = DEFAULT_VID_QUALITY;

    config.aud_mixfreq      = DEFAULT_AUD_MIXFREQ;
    config.aud_sfx          = 2;
    config.aud_music        = 2;

    vid_width               = DEFAULT_VID_WIDTH;
    vid_height              = DEFAULT_VID_HEIGHT;
    vid_fullscreen          = DEFAULT_VID_FULLSCREEN;
    vid_vsync               = DEFAULT_VID_VSYNC;
    vid_quality             = DEFAULT_VID_QUALITY;
    vid_multisampling       = 0;
    vid_aspect              = 0;
    vid_mode                = -1;
    vid_display             = -1;

    log_file                = false;

    fps_visible             = false;
    fps_counter             = 0;
    fps_timer               = 0;
    fps_timer_l             = 0;

    joystick                = NULL;
    id                      = 0;
    timer                   = SDL_GetTicks();
}

void State::log(const char *msg) {
    if (log_file) {
        FILE *fp;

        fp = fopen("liftoff.log", "a");
        fprintf(fp, "%s", msg);
        fclose(fp);
    } else {
        printf("%s", msg);
    }
}

void State::spawn(shared_ptr<Entity> e) {
    spawns.push_back(e);
}

void State::notify(uint8_t t, int16_t v) {
    messages.push_back(make_unique<Message>(t, v));
}

void State::update() {
    global_timer = static_cast<float>(SDL_GetTicks() - timer) * .05f;

    if (global_transition < 1.0f) {
        global_transition += global_timer * global_transition_speed;
    }

    if (global_transition > 1.0f) {
        global_transition = 1.0f;
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

bool State::set(int s) {
    if (id == s) return false;

    global_transition = 0;
    global_transition_speed = .02f;

    switch (s) {
        case STATE_MENU:
            timer = SDL_GetTicks();

            global_alpha = 0;
            global_timer = 0;
            global_transition_speed = .01f;

            stars_warp = true;
            stars_speed = 1.75f;
            stars_rotation_speed = .05f;

            menu = 1;
            menu_pos = 0;
            menu_selected = false;

            cam_x = 0;
            cam_y = 0;

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

        case STATE_RESTART:
            log("Restarting game...\n");

            fps_timer = 0;
            fps_timer_l = 0;
            fps_counter = 0;
            break;
    }

    id = s;

    return true;
}

int State::get() {
    return id;
}
