#include <unistd.h>

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

    config.aud_sfx          = 2;
    config.aud_music        = 2;
    config.aud_mixfreq      = DEFAULT_AUD_MIXFREQ;

    vid_cfg_multisampling   = 8;

    log_file                = E_LOG_FILE;

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

    lvl_entities            = 1;
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
    if (log_file) {
#ifdef _WIN32
        FILE *fp;

        fp = fopen("debug.log", "a");
        fprintf(fp, "%s", msg);
        fclose(fp);
#else
        printf("%s", msg);
#endif
    }
}

/*
 * load level data file
 */
bool State::load()
{
    FILE *fp = NULL;
    char fname[255], buf[1024], cmd[16], par[255];
    int m, p = 0;
    unsigned int i;
    object_t nobj;

    lvl_entities = 1;
    lvl_loaded = false;

    sprintf(fname, "%s/lvl/mission_%d.dat", engine_datadir, lvl_id);
    fp = fopen(fname, "r");
    if (fp == NULL) {
        return false;
    }

    while (!feof(fp)) {
        if (fgets(buf, 1024, fp) != NULL) {
            strcpy(cmd, "");
            strcpy(par, "");
            i = 0;
            m = 0;
            while ( (i < strlen(buf)) && (m < 4) ) {
                // forget the rest of the line
                if (buf[i] == ';') break;

                // remove unnecessary characters
                if ( buf[i] < 33 ) {
                    if (m == 0) { i++; continue; }
                    if (m == 1) { m++; continue; }
                    if (m == 2) { i++; continue; }
                    if (m == 3) { m++; continue; }
                } else {
                    if (m == 0) { p = 0; m = 1; }
                    if (m == 2) { p = 0; m = 3; }
                }

                if (m == 1) {
                    cmd[p] = buf[i];
                    p++;
                    cmd[p] = 0;
                }

                if (m == 3) {
                    if (buf[i] == '{') {
                        i++;
                        while ( (buf[i] != '}') && (i < strlen(buf)) ) {
                            par[p] = buf[i];
                            p++;
                            par[p] = 0;
                            i++;
                        }
                    } else {
                        par[p] = buf[i];
                        p++;
                        par[p] = 0;
                    }
                }

                i++;
            }

            if (cmd[0] && par[0]) {
                // music
                if (!strcmp(cmd, "soundtrack")) strcpy(lvl_music, par);

                // length
                if (!strcmp(cmd, "length")) sscanf(par, "%d", &lvl_length);

                // colliding object, obstacle
                if (!strcmp(cmd, "collider")) {
                    nobj.type = OBJ_TYPE_COLLIDER;
                    nobj.state = OBJ_STATE_IDLE;
                    nobj.speed = 0;
                    nobj.cnt = 0;
                    nobj.life_time = -1;

                    nobj.rot_x = float(rand() % 3600) * .1f;
                    nobj.rot_y = float(rand() % 3600) * .1f;
                    nobj.rot_z = float(rand() % 3600) * .1f;

                    sscanf(par, "%f,%u,%f,%f,%f,%f,%f,%f,%f,%f,%u,%u",
                        (float *)&nobj.pos_z,
                        (int *)&nobj.id,
                        (float *)&nobj.pos_x,
                        (float *)&nobj.pos_y,
                        (float *)&nobj.scale_x,
                        (float *)&nobj.scale_y,
                        (float *)&nobj.scale_z,
                        (float *)&nobj.rsp_x,
                        (float *)&nobj.rsp_y,
                        (float *)&nobj.rsp_z,
                        (int *)&nobj.life_max,
                        (int *)&nobj.money
                    );

                    nobj.life = nobj.life_max;
                    add(&nobj);
                }

                // scenery object
                if (!strcmp(cmd, "scenery")) {
                    nobj.type = OBJ_TYPE_SCENERY;
                    nobj.state = OBJ_STATE_IDLE;
                    nobj.speed = 0;
                    nobj.cnt = 0;
                    nobj.life_time = -1;

                    nobj.rot_x = float(rand() % 3600) * .1f;
                    nobj.rot_y = float(rand() % 3600) * .1f;
                    nobj.rot_z = float(rand() % 3600) * .1f;

                    sscanf(par, "%f,%u,%f,%f,%f,%f,%f,%f,%f,%f,%u",
                        (float *)&nobj.pos_z,
                        (int *)&nobj.id,
                        (float *)&nobj.pos_x,
                        (float *)&nobj.pos_y,
                        (float *)&nobj.scale_x,
                        (float *)&nobj.scale_y,
                        (float *)&nobj.scale_z,
                        (float *)&nobj.rsp_x,
                        (float *)&nobj.rsp_y,
                        (float *)&nobj.rsp_z,
                        (int *)&nobj.life_max
                    );

                    nobj.life = nobj.life_max;
                    add(&nobj);
                }
            }
        }
    }
    
    return true;
}

/*
 * add new entity to scenery
 */
bool State::add(object_t *n_obj)
{

    if (lvl_entities == E_MAX_OBJECTS) {
        return false;
    } else {
        memcpy(&objects[lvl_entities], n_obj, sizeof(object_t));
        lvl_entities++;
    }

    return true;
}

/*
 * remove entity from scenery
 */
void State::remove(int oid)
{
    lvl_entities--;

    if (oid < lvl_entities) {
        if (player == lvl_entities) player = oid;
        memcpy(&objects[oid], &objects[lvl_entities], sizeof(object_t));
    }
}

/*
 * object destroyed, remove it and possibly spawn a new one
 */
void State::explode(int oid)
{
    int sangle;

    sangle = int(objects[oid].pos_x / 2.8f);
    if (sangle < 0) sangle += 360;

    if (objects[player].target == oid) {
        objects[player].target = -1;
    }

    if (objects[oid].money > 0) {
        addMessage(objects[oid].money, MSG_MONEY);
        objects[player].money += objects[oid].money;
    }

    switch (objects[oid].id) {
        case OBJ_CARGO_1:
            // cargo pod explodes and leaves a power up
            audio.playSample(5, 192, sangle);
            objects[oid].id        = OBJ_POWERUP_1;
            objects[oid].energy    = 250 + rand() % 250;
            objects[oid].life      = 999;
            objects[oid].life_time = 500;
            objects[oid].life_max  = 0;
            objects[oid].cnt       = 0;
            objects[oid].money     = 0;
            return;

        case OBJ_POWERUP_1:
            // power up was picked up and turns into a non-colliding object
            audio.playSample(9, 255, sangle);
            addMessage(objects[oid].energy, MSG_ENERGY);
            objects[player].powerup = objects[oid].id;
            objects[player].energy += objects[oid].energy;
            objects[oid].type = OBJ_TYPE_SCENERY;
            objects[oid].state = OBJ_STATE_FADING;
            return;

        default:
            // explosion
            audio.playSample(6, 192, sangle);
            remove(oid);
    }
}

/*
 * sort objects by Z position
 */
void State::sort()
{
    int i, j;
    object_t tmpobj;

    for (i=0; i<lvl_entities-1; i++) {
        if (objects[i].state == OBJ_STATE_IDLE) continue;

        for (j=i+1; j<lvl_entities; j++) {
            if (objects[j].state == OBJ_STATE_IDLE) continue;

            if (objects[i].pos_z > objects[j].pos_z) {
                if (i == player) {
                    player = j;
                } else if (j == player) {
                    player = i;
                }

                tmpobj = objects[j];
                objects[j] = objects[i];
                objects[i] = tmpobj;
            }
        }
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

    sprintf(msg, "Setting global state %d\n", s);
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
            player = 0;
            engine_restart = false;

            objects[player].type  = OBJ_TYPE_PLAYER;
            objects[player].state = OBJ_STATE_ACTIVE;
            objects[player].id    = OBJ_PLAYER;

            objects[player].rot_x = .0f;
            objects[player].rot_y = .0f;
            objects[player].rot_z = 250.0f;

            objects[player].rsp_x = .0f;
            objects[player].rsp_y = .0f;
            objects[player].rsp_z = .0f;

            objects[player].s_x   = .0f;
            objects[player].s_y   = .0f;
            objects[player].s_z   = .0f;

            objects[player].a_x   = .0f;
            objects[player].a_y   = .0f;
            objects[player].a_z   = .0f;

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
            lvl_entities = 0;

            sprintf(msg, "Loading 'lvl/mission_%d.dat'... ", lvl_id);
            log(msg);

            if (load()) {
                sprintf(msg, "ok (%d objects spawned)\n", lvl_entities);
                log(msg);

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

                objects[player].target = -1;
                objects[player].life = 0;
                objects[player].money = 0;
                objects[player].energy = 0;
                objects[player].powerup = OBJ_POWERUP_0;

                audio.stopMusic(1000);
                if (strlen(lvl_music)) {
                    audio.music[1] = audio.loadMusic(lvl_music);
                }
            } else {
                log("failed\n");
                set(STATE_MENU);
            }
            break;

        case STATE_GAME_LOOP:
            log("Entering game loop...\n");
            menu = 0;

            stars_warp = false;
            stars_rotation = false;
            stars_speed = .35f;

            objects[player].target = -1;
            objects[player].life = 1;
            objects[player].speed = E_BASE_SPEED;

            objects[player].pos_x = .0f;
            objects[player].pos_y = -90.0f;
            objects[player].pos_z = 50.0f;

            objects[player].rot_x = .0f;
            objects[player].rot_y = .0f;
            objects[player].rot_z = .0f;

            objects[player].rsp_x = .0f;
            objects[player].rsp_y = .0f;
            objects[player].rsp_z = .0f;

            objects[player].s_x = .0f;
            objects[player].s_y = .0f;
            objects[player].s_z = .0f;

            objects[player].a_x = .0f;
            objects[player].a_y = .0f;
            objects[player].a_z = .0f;

            cam_x = -250.0f;
            if ((rand() % 2) == 1) cam_x = -cam_x;
            cam_y = -400.0f;
            if ((rand() % 2) == 1) cam_y = -cam_y;
            cam_y_offset = 35.0f;
            cam_speed = .015f;

            audio.playSampleLoop(3, 1000);
            break;

        case STATE_GAME_QUIT:
            log("Quitting game loop.\n");
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
