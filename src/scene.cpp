#include "scene.hpp"

Scene::Scene(State &s) : 
    state(s),
    player(new Player()),
    skybox(new Skybox())
{
    state.player = player;
}

Scene::~Scene()
{
    if (state.audio.volume_music) {
        Mix_HaltMusic();
        Mix_FreeMusic(state.audio.music[0]);
    }

    if (state.audio.volume_sfx) {
        Mix_FreeChunk(state.audio.sample[10]);
        Mix_FreeChunk(state.audio.sample[9]);
        Mix_FreeChunk(state.audio.sample[8]);
        Mix_FreeChunk(state.audio.sample[7]);
        Mix_FreeChunk(state.audio.sample[6]);
        Mix_FreeChunk(state.audio.sample[5]);
        Mix_FreeChunk(state.audio.sample[4]);
        Mix_FreeChunk(state.audio.sample[3]);
        Mix_FreeChunk(state.audio.sample[2]);
        Mix_FreeChunk(state.audio.sample[1]);
        Mix_FreeChunk(state.audio.sample[0]);
    }

    state.textures.clear();
    state.shaders.clear();
    state.models.clear();
    state.fonts.clear();
}

/*
 * load game data
 */
void Scene::load()
{
    state.log("Loading 'title.tga'\n");
    state.textures.insert(make_pair(T_TITLE, new Texture(
        string(state.dir_resources).append("/textures/title.tga")
    )));

    state.log("Loading 'font.tga'\n");
    state.textures.insert(make_pair(T_FONT, new Texture(
        string(state.dir_resources).append("/textures/font.tga")
    )));

    state.log("Loading 'menu_1.tga'\n");
    state.textures.insert(make_pair(T_MENU_1, new Texture(
        string(state.dir_resources).append("/textures/menu_1.tga")
    )));

    state.log("Loading 'menu_2.tga'\n");
    state.textures.insert(make_pair(T_MENU_2, new Texture(
        string(state.dir_resources).append("/textures/menu_2.tga")
    )));

    state.log("Loading 'hud_1.tga'\n");
    state.textures.insert(make_pair(T_HUD_1, new Texture(
        string(state.dir_resources).append("/textures/hud_1.tga")
    )));

    state.log("Loading 'hud_2.tga'\n");
    state.textures.insert(make_pair(T_HUD_2, new Texture(
        string(state.dir_resources).append("/textures/hud_2.tga")
    )));

    state.log("Loading 'hud_3.tga'\n");
    state.textures.insert(make_pair(T_HUD_3, new Texture(
        string(state.dir_resources).append("/textures/hud_3.tga")
    )));

    state.log("Loading 'hud_4.tga'\n");
    state.textures.insert(make_pair(T_HUD_4, new Texture(
        string(state.dir_resources).append("/textures/hud_4.tga")
    )));

    state.log("Loading 'star.tga'\n");
    state.textures.insert(make_pair(T_STAR, new Texture(
        string(state.dir_resources).append("/textures/star.tga")
    )));

    state.log("Loading 'missile_1.tga'\n");
    state.textures.insert(make_pair(T_MISSILE_1, new Texture(
        string(state.dir_resources).append("/textures/missile_1.tga")
    )));

    state.log("Loading 'explosion_1.tga'\n");
    state.textures.insert(make_pair(T_EXPLOSION_1, new Texture(
        string(state.dir_resources).append("/textures/explosion_1.tga")
    )));

    state.log("Loading 'explosion_2.tga'\n");
    state.textures.insert(make_pair(T_EXPLOSION_2, new Texture(
        string(state.dir_resources).append("/textures/explosion_2.tga")
    )));

    state.log("Loading 'jet_exhaust.tga'\n");
    state.textures.insert(make_pair(T_JET_EXHAUST, new Texture(
        string(state.dir_resources).append("/textures/jet_exhaust.tga")
    )));

    state.log("Loading 'background_1.tga'\n");
    state.textures.insert(make_pair(T_BACKGROUND_1, new Texture(
        string(state.dir_resources).append("/textures/background_1.tga")
    )));

    state.log("Loading 'glow_1.tga'\n");
    state.textures.insert(make_pair(T_GLOW, new Texture(
        string(state.dir_resources).append("/textures/glow_1.tga")
    )));

    // fonts

    state.log("Loading font 'zekton'\n");
    state.fonts.insert(make_pair(F_ZEKTON, new Font(
        string(state.dir_resources).append("/fonts/zekton")
    )));

    // shaders

    state.log("Loading shader 'texture_1'\n");
    state.shaders.insert(make_pair(S_TEXTURE_1, new Shader(
        string(state.dir_resources).append("/shaders/texture_1.glvs"),
        string(state.dir_resources).append("/shaders/texture_1.glfs")
    )));

    state.log("Loading shader 'basic_1'\n");
    state.shaders.insert(make_pair(S_BASIC_1, new Shader(
        string(state.dir_resources).append("/shaders/basic_1.glvs"),
        string(state.dir_resources).append("/shaders/basic_1.glfs")
    )));

    // models

    state.log("Loading model 'ship_1'\n");
    state.models.insert(make_pair(OBJ_PLAYER, new Model(
        make_shared<Object>(string(state.dir_resources).append("/models/ship_1.obj")),
        make_shared<Texture>(string(state.dir_resources).append("/models/ship_1.tga")),
        make_shared<Shader>(*state.shaders[S_BASIC_1])
    )));

    state.log("Loading model 'asteroid_1'\n");
    state.models.insert(make_pair(OBJ_ASTEROID_1, new Model(
        make_shared<Object>(string(state.dir_resources).append("/models/asteroid_1.obj")),
        make_shared<Texture>(string(state.dir_resources).append("/models/asteroid_1.tga")),
        make_shared<Shader>(*state.shaders[S_BASIC_1])
    )));

    state.log("Loading model 'debris_1'\n");
    state.models.insert(make_pair(OBJ_DEBRIS_1, new Model(
        make_shared<Object>(string(state.dir_resources).append("/models/debris_1.obj")),
        make_shared<Texture>(string(state.dir_resources).append("/models/debris_1.tga")),
        make_shared<Shader>(*state.shaders[S_BASIC_1])
    )));

    state.log("Loading model 'cargo_1'\n");
    state.models.insert(make_pair(OBJ_CARGO_1, new Model(
        make_shared<Object>(string(state.dir_resources).append("/models/cargo_1.obj")),
        make_shared<Texture>(string(state.dir_resources).append("/models/cargo_1.tga")),
        make_shared<Shader>(*state.shaders[S_BASIC_1])
    )));

    // sound effects

    state.audio.sample[0]  = state.audio.loadSample("menu_1.wav");
    state.audio.sample[1]  = state.audio.loadSample("menu_2.wav");
    state.audio.sample[2]  = state.audio.loadSample("gun_1.wav");
    state.audio.sample[3]  = state.audio.loadSample("engine_1.wav");
    state.audio.sample[4]  = state.audio.loadSample("explosion_1.wav");
    state.audio.sample[5]  = state.audio.loadSample("explosion_2.wav");
    state.audio.sample[6]  = state.audio.loadSample("explosion_3.wav");
    state.audio.sample[7]  = state.audio.loadSample("shield_1.wav");
    state.audio.sample[8]  = state.audio.loadSample("engine_2.wav");
    state.audio.sample[9]  = state.audio.loadSample("powerup_1.wav");
    state.audio.sample[10] = state.audio.loadSample("engine_3.wav");
    state.audio.sample[11] = state.audio.loadSample("logo.wav");

    // music

    state.audio.music[0]   = state.audio.loadMusic("music_title.ogg");
}

/*
 * load level data file
 */
bool Scene::loadLevel()
{
    FILE *fp = NULL;
    char msg[255], fname[255], buf[1024], cmd[16], par[255];

    int i, money, life, m, p = 0;
    unsigned short e_obj;
    float p_x, p_y, p_z;
    float r_x, r_y, r_z;
    float s_x, s_y, s_z;
    float w_x, w_y, w_z;

    state.lvl_loaded = false;

    state.entities.clear();
    state.entities.push_back(player);

    sprintf(fname, "%s/levels/mission_%d.dat", state.dir_resources, state.lvl_id);
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

            while ((i < strlen(buf)) && (m < 4)) {
                // forget the rest of the line
                if (buf[i] == ';') break;

                // remove unnecessary characters
                if (buf[i] < 33) {
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
                        while ((buf[i] != '}') && (i < strlen(buf))) {
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
                if (!strcmp(cmd, "soundtrack")) strcpy(state.lvl_music, par);

                // length
                if (!strcmp(cmd, "length")) sscanf(par, "%d", &state.lvl_length);

                // colliding object, obstacle
                if (!strcmp(cmd, "collider")) {
                    r_x = float(rand() % 3600) * .1f;
                    r_y = float(rand() % 3600) * .1f;
                    r_z = float(rand() % 3600) * .1f;

                    sscanf(par, "%f,%u,%f,%f,%f,%f,%f,%f,%f,%f,%u,%u",
                        (float *) &p_z,
                        (int *)   &e_obj,
                        (float *) &p_x,
                        (float *) &p_y,
                        (float *) &s_x,
                        (float *) &s_y,
                        (float *) &s_z,
                        (float *) &w_x,
                        (float *) &w_y,
                        (float *) &w_z,
                        (int *)   &life,
                        (int *)   &money
                    );

                    switch (e_obj) {
                        case OBJ_ASTEROID_1:
                            {
                                auto asteroid = make_shared<Asteroid>();

                                asteroid->setPos(p_x, p_y, p_z);
                                asteroid->setRot(r_x, r_y, r_z);
                                asteroid->setScale(s_x, s_y, s_z);
                                asteroid->setSpin(w_x, w_y, w_z);
                                asteroid->setLife(life);

                                state.entities.push_back(asteroid);
                            }
                            break;

                        case OBJ_CARGO_1:
                            {
                                auto cargo = make_shared<Cargo>();

                                cargo->setPos(p_x, p_y, p_z);
                                cargo->setRot(r_x, r_y, r_z);
                                cargo->setScale(s_x, s_y, s_z);
                                cargo->setSpin(w_x, w_y, w_z);
                                cargo->setLife(life);

                                state.entities.push_back(cargo);
                            }
                            break;
                    }
                }

                // scenery object
                if (!strcmp(cmd, "scenery")) {
                    r_x = float(rand() % 360);
                    r_y = float(rand() % 360);
                    r_z = float(rand() % 360);

                    sscanf(par, "%f,%u,%f,%f,%f,%f,%f,%f,%f,%f,%u",
                        (float *) &p_z,
                        (int *)   &e_obj,
                        (float *) &p_x,
                        (float *) &p_y,
                        (float *) &s_x,
                        (float *) &s_y,
                        (float *) &s_z,
                        (float *) &w_x,
                        (float *) &w_y,
                        (float *) &w_z,
                        (int *)   &life
                    );

                    switch (e_obj) {
                        case OBJ_ASTEROID_1:
                            {
                                auto asteroid = make_shared<Asteroid>();

                                asteroid->setType(E_TYPE_SCENERY);
                                asteroid->setPos(p_x, p_y, p_z);
                                asteroid->setRot(r_x, r_y, r_z);
                                asteroid->setScale(s_x, s_y, s_z);
                                asteroid->setSpin(w_x, w_y, w_z);
                                asteroid->setLife(life);

                                state.entities.push_back(asteroid);
                            }
                            break;
                    }
                }
            }
        }
    }
    
    return true;
}

/*
 * draw text
 */
void Scene::drawText(const char *text, float x, float y, float z, float size, float r, float g, float b, float a)
{
    int strLenth = strlen(text);
    char letter, lastletter = 0;
    GLfloat x1 = .0f, w, y1, y2, wy;

    glColor4f(r, g, b, a);
    state.textures[T_FONT]->bind();

    glPushMatrix();
    glTranslatef(x, y, z);

    for (int i=0; i<strLenth; i++) {
        letter = text[i];
        w = 0;
        wy = 0;

        if (letter == 10) {
            glPopMatrix();
            glPushMatrix();
            y-=(size/280)*1.05f;
            glTranslatef(x, y, z);
        } else if (letter == 32) {
            glTranslatef(size/1000, 0, 0);
        } else {
            if (letter <= 58) {
                // "0" .. "9", ":", "/", "+"
                y1 = .59f;
            } else if (letter >=97) {
                // "a" .. "z"
                y1 = .3f;
            } else {
                // "A" .. "Z"
                y1 = .004f;
            }
            y2 = y1 + .25f;

            switch(letter) {
                case 34:  x1 = 0.435f; w = 0.0170f; break;            // "
                case 36:  x1 = 0.600f; w = 0.0285f; break;            // $
                case 43:  x1 = 0.558f; w = 0.0200f; wy = .03f; break; // +
                case 45:  x1 = 0.509f; w = 0.0161f; wy = .03f; break; // -
                case 46:  x1 = 0.480f; w = 0.0120f; break;            // .
                case 47:  x1 = 0.384f; w = 0.0174f; break;            // /
                case 48:  x1 = 0.300f; w = 0.0320f; break;            // 0
                case 49:  x1 = 0.008f; w = 0.0130f; break;            // 1
                case 50:  x1 = 0.026f; w = 0.0274f; break;            // 2
                case 51:  x1 = 0.062f; w = 0.0255f; break;            // 3
                case 52:  x1 = 0.093f; w = 0.0320f; break;            // 4
                case 53:  x1 = 0.126f; w = 0.0286f; break;            // 5
                case 54:  x1 = 0.159f; w = 0.0305f; break;            // 6
                case 55:  x1 = 0.196f; w = 0.0230f; break;            // 7
                case 56:  x1 = 0.226f; w = 0.0320f; break;            // 8
                case 57:  x1 = 0.264f; w = 0.0310f; break;            // 9
                case 58:  x1 = 0.495f; w = 0.0120f; wy = .06f; break; // :
                case 65:  x1 = 0.006f; w = 0.0415f; break;            // A
                case 66:  x1 = 0.050f; w = 0.0310f; break;            // B
                case 67:  x1 = 0.086f; w = 0.0285f; break;            // C
                case 68:  x1 = 0.121f; w = 0.0370f; break;            // D
                case 69:  x1 = 0.163f; w = 0.0278f; break;            // E
                case 70:  x1 = 0.195f; w = 0.0266f; break;            // F
                case 71:  x1 = 0.230f; w = 0.0335f; break;            // G
                case 72:  x1 = 0.270f; w = 0.0342f; break;            // H
                case 73:  x1 = 0.308f; w = 0.0066f; break;            // I
                case 74:  x1 = 0.320f; w = 0.0200f; break;            // J
                case 75:  x1 = 0.340f; w = 0.0325f; break;            // K
                case 76:  x1 = 0.382f; w = 0.0274f; break;            // L
                case 77:  x1 = 0.413f; w = 0.0418f; break;            // M
                case 78:  x1 = 0.460f; w = 0.0353f; break;            // N
                case 79:  x1 = 0.500f; w = 0.0362f; break;            // O
                case 80:  x1 = 0.540f; w = 0.0302f; break;            // P
                case 81:  x1 = 0.576f; w = 0.0364f; break;            // Q
                case 82:  x1 = 0.617f; w = 0.0316f; break;            // R
                case 83:  x1 = 0.654f; w = 0.0364f; break;            // S
                case 84:  x1 = 0.694f; w = 0.0322f; break;            // T
                case 85:  x1 = 0.730f; w = 0.0356f; break;            // U
                case 86:  x1 = 0.768f; w = 0.0410f; break;            // V
                case 87:  x1 = 0.812f; w = 0.0555f; break;            // W
                case 88:  x1 = 0.870f; w = 0.0380f; break;            // X
                case 89:  x1 = 0.910f; w = 0.0420f; break;            // Y
                case 90:  x1 = 0.954f; w = 0.0376f; break;            // Z
                case 120: x1 = 0.697f; w = 0.0310f; break;            // x
            }

            if ((lastletter == 84) && (letter == 65)) {
                glTranslatef(-(size/1500), 0, 0);
            } else if ((lastletter == 65) && (letter == 84)) {
                glTranslatef(-(size/1500), 0, 0);
            } else if ((lastletter == 47) && (letter == 65)) {
                glTranslatef(-(size/1500), 0, 0);
            }

            glBegin(GL_QUADS);
              glTexCoord2f(x1, y2+wy);
              glVertex3f(0, 0, 0);

              glTexCoord2f(x1+w, y2+wy);
              glVertex3f((size/14)*w, 0, 0);

              glTexCoord2f(x1+w, y1+wy);
              glVertex3f((size/14)*w, size/280, 0);

              glTexCoord2f(x1, y1+wy);
              glVertex3f(0, size/280, 0);
            glEnd();

            glTranslatef((size/14)*(w+.0065f), 0, 0) ;
        }

        lastletter = letter;
     }

     glPopMatrix();
}

/*
 * draw text using absolute coordinates
 */
void Scene::drawTextA(const char *text, float x, float y, float z, float size, float r, float g, float b, float a)
{
    drawText(text, x + (-5.97f * state.vid_aspect), -y + 5.69f, z, size, r, g, b, a);
}

/*
 * display video infos (FPS, MSAA level)
 */
void Scene::drawVideoInfos()
{
    static char txt[16];

    if (state.fps_timer_l > 0) {
        state.fps_timer += state.timer - state.fps_timer_l;

        if (state.fps_timer > 1000) {
            state.fps = float(state.fps_counter) / (float(state.fps_timer) * .001f);
            state.fps_ready = true;
            state.fps_counter = 0;
            state.fps_timer = 0;
        }
    }

    state.fps_counter++;
    state.fps_timer_l = state.timer;

    glLoadIdentity();

    // FPS

    if (state.fps_ready) {
        sprintf(txt, "FPS: %d", int(round(state.fps)));
    } else {
        sprintf(txt, "FPS: WAIT...");
    }

    drawTextA(txt, -.45f, -.25f, -10.0f, 65, .6f, .6f, .6f, 1.0f);

    // MSAA

    if (state.vid_multisampling > 0) {
        sprintf(txt, "MSAA: %dx", int(state.vid_multisampling));
    } else {
        sprintf(txt, "MSAA: OFF");
    }

    drawTextA(txt, -.45f, .05f, -10.0f, 65, .6f, .6f, .6f, 1.0f);
}

/*
 * draw title
 */
void Scene::drawTitle()
{
    float sc, a = state.menu_title_pos * .01f;

    glDisable(GL_DEPTH_TEST);

    glLoadIdentity();
    state.textures[T_TITLE]->bind();
    glPushMatrix();

    if (state.get() == STATE_MENU) {
        glTranslatef(0, 7.75f-((a*2)*(a*2)), -10.0f);
    } else {
        glTranslatef(-16.0f+((a*4)*(a*4)), 3.75f, -10.0f);
    }

    glBegin (GL_QUADS);
      glColor4f(a*.075f, a*.075f, a*.075f, a);
      glTexCoord2f (0, 0);
      glVertex3f (-4, 1, 0);

      glTexCoord2f (1, 0);
      glVertex3f (4, 1, 0);

      glColor4f(a, a, a, a);
      glTexCoord2f (1, .6f);
      glVertex3f (4, -.4f, 0);

      glTexCoord2f (0, .6f);
      glVertex3f (-4, -.4f, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();

    if (state.get() == STATE_MENU) {
        sc = a*a*1.5f;
        glTranslatef(0, 2.65f, -30+(a*20));
    } else {
        sc = 1.5f;
        glTranslatef(16.0f-(a*4)*(a*4), 2.65f, -10.02929f);
    }

    glColor4f(a, a, a, a);
    glBegin (GL_QUADS);
      glTexCoord2f (0, .6f);
      glVertex3f (-4*sc, .75f*sc, 0);

      glTexCoord2f (1, .6f);
      glVertex3f (4*sc, .75f*sc, 0);

      glTexCoord2f (1, 1);
      glVertex3f (4*sc, 0, 0);

      glTexCoord2f (0, 1);
      glVertex3f (-4*sc, 0, 0);
    glEnd();
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
}

/*
 * draw menu
 */
void Scene::drawMenu()
{
    int i, numentries;
    float mx, my, mh, mf, mo;
    float m_a = state.global_alpha;
    char *mtxt[5];
    char msg[255];

    for (i=0; i<5; i++) {
        mtxt[i] = (char*)malloc(sizeof(char)*64);
    }

    switch (state.menu) {
        case 1: // main menu
            numentries = 3;
            mx = -1.26f;
            my = -.25f;
            mh = 1.0f;
            mf = 120;
            mo = .27f;

            strcpy(mtxt[0], "LAUNCH");
            strcpy(mtxt[1], "SETTINGS");
            strcpy(mtxt[2], "QUIT");

            if (state.menu_selected) {
                switch (state.menu_pos) {
                    case 0: // launch
                        sprintf(msg, "Loading 'lvl/mission_%d.dat'... ", state.lvl_id);
                        state.log(msg);

                        if (loadLevel()) {
                            state.log("ok\n");
                            state.set(STATE_GAME_START);

                            player->setEnergy(-200);
                            player->setLife(player->getLifeMaximum());
                            player->setMoney(0);
                            player->collect(OBJ_POWERUP_0);
                        } else {
                            state.log("failed\n");
                        }

                        break;

                    case 1: // enter settings
                        if (state.menu != 2) {
                            state.menu = 2;
                            state.menu_pos = 0;
                            state.menu_selected = false;
                            drawMenu();
                            return;
                        }
                        break;

                    case 2: // quit game
                        state.set(STATE_QUIT);
                        break;
                }
            }
            break;

        case 2: // settings submenu
            numentries = 4;
            mx = -1.26f;
            my = 0.015f;
            mh = .75f;
            mf = 105;
            mo = .175f;

            strcpy(mtxt[0], "VIDEO");
            strcpy(mtxt[1], "AUDIO");
            strcpy(mtxt[2], "CANCEL");
            strcpy(mtxt[3], "ACCEPT");

            if (state.menu_selected) {
                switch (state.menu_pos) {
                    case 0: // enter video settings
                        state.menu = 3;
                        state.menu_pos = 0;
                        break;

                    case 1: // enter audio settings
                        state.menu = 4;
                        state.menu_pos = 0;
                        break;

                    case 2: // cancel
                        state.menu = 1;
                        state.menu_pos = 1;
                        state.config.aud_sfx = state.audio.volume_sfx;
                        state.config.aud_music = state.audio.volume_music;
                        state.config.aud_mixfreq = state.audio.mixer_frequency;
                        for (int i = 0; i < state.vid_cap_modes_num; i++) {
                            if ( (state.vid_width  == state.vid_cap_modes[i].w) &&
                                 (state.vid_height == state.vid_cap_modes[i].h) ) {
                                 state.vid_mode = i;
                            }
                        }
                        state.config.vid_aspect = state.vid_aspect_mode;
                        state.config.vid_fullscreen = state.vid_fullscreen;
                        state.config.vid_vsync = state.vid_vsync;
                        break;

                    case 3: // accept
                        state.menu = 1;
                        state.menu_pos = 1;
                        state.engine_restart = true;
                        break;
                }
            }

            break;

        case 3: // video settings
            numentries = 5;
            mx = -1.26f;
            my = .135f;
            mh = .625f;
            mf = 70;
            mo = .32f;

            sprintf(mtxt[0], "VIDEO MODE:\n     %dx%d",
              state.vid_cap_modes[state.vid_mode].w,
              state.vid_cap_modes[state.vid_mode].h);

            switch(state.config.vid_aspect) {
                case 1:
                    strcpy(mtxt[1], "ASPECT RATIO:\n     4:3");
                    break;
                case 2:
                    strcpy(mtxt[1], "ASPECT RATIO:\n     16:9");
                    break;
                case 3:
                    strcpy(mtxt[1], "ASPECT RATIO:\n     16:10");
                    break;
                default:
                    strcpy(mtxt[1], "ASPECT RATIO:\n     AUTO");
            }

            if (state.config.vid_fullscreen)
                strcpy(mtxt[2], "FULL SCREEN:\n     ENABLED");
            else
                strcpy(mtxt[2], "FULL SCREEN:\n     DISABLED");

            if (state.config.vid_vsync)
                strcpy(mtxt[3], "VERTICAL SYNC:\n     ENABLED");
            else
                strcpy(mtxt[3], "VERTICAL SYNC:\n     DISABLED");

            strcpy(mtxt[4], "RETURN");

            if (state.menu_selected) {
                switch (state.menu_pos) {
                    case 0: // toggle video mode
                        state.vid_mode--;

                        if (state.vid_mode < 0) {
                            state.vid_mode = state.vid_cap_modes_num - 1;
                        }
                        break;

                    case 1: // toggle aspect ratio
                        state.config.vid_aspect++;

                        if (state.config.vid_aspect > 3) {
                            state.config.vid_aspect = 0;
                        }
                        break;

                    case 2: // toggle fullscreen on/off
                        state.config.vid_fullscreen = !state.config.vid_fullscreen;
                        break;

                    case 3: // toggle vsync on/off
                        state.config.vid_vsync = !state.config.vid_vsync;
                        break;

                    case 4: // return
                        state.menu_pos = 0;
                        state.menu = 2;
                        break;
                }
            }
            break;

        case 4: // audio settings
            numentries = 4;
            mx = -1.26f;
            my = 0.015f;
            mh = .75f;
            mf = 75;
            mo = .375f;

            switch(state.config.aud_sfx) {
                case 0:
                    strcpy(mtxt[0], "SOUND FX:\n     MUTED");
                    break;
                case 1:

                    strcpy(mtxt[0], "SOUND FX:\n     MINIMUM");
                    break;
                case 2:

                    strcpy(mtxt[0], "SOUND FX:\n     MEDIUM");
                    break;
                case 3:

                    strcpy(mtxt[0], "SOUND FX:\n     MAXIMUM");
                    break;

                default:
                    strcpy(mtxt[0], "SOUND FX:\n     DISABLED");
            }

            switch(state.config.aud_music) {
                case 0:
                    strcpy(mtxt[1], "MUSIC:\n     MUTED");
                    break;
                case 1:

                    strcpy(mtxt[1], "MUSIC:\n     MINIMUM");
                    break;
                case 2:

                    strcpy(mtxt[1], "MUSIC:\n     MEDIUM");
                    break;
                case 3:

                    strcpy(mtxt[1], "MUSIC:\n     MAXIMUM");
                    break;

                default:
                    strcpy(mtxt[1], "MUSIC:\n     DISABLED");
            }

            sprintf(mtxt[2], "MIXER QUALITY:\n     %d HZ", state.config.aud_mixfreq);
            strcpy(mtxt[3], "RETURN");

            if (state.menu_selected) {
                switch (state.menu_pos) {
                    case 0: // toggle SFX volume
                        if (state.config.aud_sfx != -1) {
                            state.config.aud_sfx++;

                            if (state.config.aud_sfx > 3) {
                                state.config.aud_sfx = 0;
                            }
                        }
                        break;

                    case 1: // toggle music volume
                        if (state.config.aud_music != -1) {
                            state.config.aud_music++;

                            if (state.config.aud_music > 3) {
                                state.config.aud_music = 0;
                            }
                        }
                        break;

                    case 2: // toggle mixer frequency
                        if (state.config.aud_mixfreq == 22050) {
                            state.config.aud_mixfreq = 44100;
                        } else {
                            state.config.aud_mixfreq = 22050;
                        }
                        break;

                    case 3: // return
                        state.menu = 2;
                        state.menu_pos = 1;
                        break;
                }
            }

            break;

        default:
            return;
    }

    if (state.menu_selected) {
        state.menu_selected = false;
        drawMenu();
        return;
    }

    if (state.menu_pos >= numentries) {
        state.menu_pos = numentries-1;
    }

    if (state.menu_pos < 0) {
        state.menu_pos = 0;
    }

    if (state.get() >= STATE_GAME_START) {
        m_a = (float)state.menu_title_pos;
    }

    // draw menu background
    glLoadIdentity();
    glPushMatrix();
    state.textures[T_MENU_1]->bind();
    glTranslatef(0, -0.8f, -10);
    glScalef(1, 1, 1);
    glColor4f(1, 1, 1, .01f*m_a);
    glBegin (GL_QUADS);
      glTexCoord2f (0, 0);
      glVertex3f (-5, 1.9f, 0);

      glColor4f(0.75f, 0.75f, 0.75f, .01f*m_a);
      glTexCoord2f (1, 0);
      glVertex3f (5, 1.9f, 0);

      glColor4f(0.5f, 0.5f, 0.5f, .01f*m_a);
      glTexCoord2f (1, 1);
      glVertex3f (5, -1.9f, 0);

      glColor4f(0.75f, 0.75f, 0.75f, .01f*m_a);
      glTexCoord2f (0, 1);
      glVertex3f (-5, -1.9f, 0);
    glEnd();
    glPopMatrix();

    drawText("VECTOR ZERO MK. IX \"REDUX\"", -.1f, -2.3f, -10.0f, 65, 1, .9f, .85f, .0045f*m_a);

    // draw menu items
    for (i=0; i<numentries; i++) {
        drawText(mtxt[i], mx-3.0f, my-(i*mh)+mo, -9.9f, mf, 1, .8f, .55f, .0085f*m_a);

        if (i == state.menu_pos) {
            glLoadIdentity();
            glPushMatrix();
            state.textures[T_MENU_2]->bind();
            glTranslatef(-4.9f, (my-(i*mh)), -9.95f);
            glColor4f(.01f*m_a, .01f*m_a, .01f*m_a, 0.2f*m_a);
            glBegin (GL_QUADS);
              glTexCoord2f (0.2f, 0.04f);
              glVertex3f (0, 0, 0);

              glTexCoord2f (0.2f, 0.96f);
              glVertex3f (0, mh, 0);

              glTexCoord2f (1, 0.96f);
              glVertex3f (3.8f, mh, 0);

              glTexCoord2f (1, 0.04f);
              glVertex3f (3.8f, 0, 0);
            glEnd();
            glPopMatrix();
        }
    }
}

/*
 * draw game scene
 */
void Scene::drawScene()
{
    int i;
    float alpha;

    std::sort(state.entities.begin(), state.entities.end(), Entity::sort);

    auto e = state.entities.begin();

    while (e != state.entities.end()) {
        if ((*e)->isIdle() == false) {
            (*e)->draw(state);
            (*e)->drawCrosshair(state, *e);
        }

        ++e;
    }
}

/*
 * draw head-up-display (energy, shields, money)
 */
void Scene::drawDisplay()
{
    int i, s, e;
    static float alpha = 1.0f;
    float t;
    char msg[16];

    switch (state.get()) {

        case STATE_GAME_START:
            t = (-6.413f * state.vid_aspect - state.hud_x) * .055f * state.timer_adjustment;
            state.hud_x += t;
            t = (4.905f + state.hud_y) * .055f * state.timer_adjustment;
            state.hud_y -= t;
            alpha = float(100 - state.menu_title_pos) * .01f;
            break;

        case STATE_GAME_NEXTLEVEL:
        case STATE_GAME_QUIT:
            if (state.menu_title_pos > 0) {
                state.hud_x -= state.menu_title_pos * .001f;
                state.hud_y -= state.menu_title_pos * .001f;
            }
            alpha = float(100 - state.menu_title_pos) * .01f;
            break;

        case STATE_GAME_LOOP:
            if (player->isAlive()) {
                state.hud_x = -6.413f * state.vid_aspect;
                state.hud_y = -4.905f;
            } else {
                state.hud_x -= state.timer_adjustment * .01f;
                state.hud_y -= state.timer_adjustment * .01f;
                alpha -= state.timer_adjustment * .01f;
            }
            break;

        default:
            state.hud_x = .0f;
            state.hud_y = -.8f;
            alpha = .01f * (100.0f - float(state.menu_title_pos));
    }

    glLoadIdentity();

    // lower right screen

    state.textures[T_MENU_1]->bind();

    glPushMatrix();
    glTranslatef(-state.hud_x - state.tilt_x * .01f, state.hud_y - state.tilt_y * .01f, -10);
    glBegin (GL_QUADS);
      glColor4f(1, 1, 1, alpha * .5f);
      glTexCoord2i(0, 0);
      glVertex3f(-5.1f, -.3f, 0);

      glColor4f(0.75f, 0.75f, 0.75f, alpha * .5f);
      glTexCoord2i(1, 0);
      glVertex3f(4.9f, -.3f, 0);

      glColor4f(0.5f, 0.5f, 0.5f, alpha * .5f);
      glTexCoord2i(1, 1);
      glVertex3f(4.9f, -4.1f, 0);

      glColor4f(0.75f, 0.75f, 0.75f, alpha * .5f);
      glTexCoord2i(0, 1);
      glVertex3f(-5.1f, -4.1f, 0);
    glEnd();

    // money

    sprintf(msg, "%d $", player->getMoney());
    drawText(msg, -4.6f, -1.2f, 0, 130, 1.0f, 1.0f, .3f, alpha * .85f);
    glPopMatrix();

    glLoadIdentity();

    // lower left screen

    state.textures[T_MENU_1]->bind();

    glPushMatrix();
    glTranslatef(state.hud_x - state.tilt_x * .01f, state.hud_y - state.tilt_y * .01f, -10);
    glColor4f(1, 1, 1, alpha * .5f);
    glBegin (GL_QUADS);
      glTexCoord2i(0, 0);
      glVertex3f(-5.1f, -.3f, 0);

      glTexCoord2i(1, 0);
      glVertex3f(4.9f, -.3f, 0);

      glTexCoord2i(1, 1);
      glVertex3f(4.9f, -4.1f, 0);

      glTexCoord2i(0, 1);
      glVertex3f(-5.1f, -4.1f, 0);
    glEnd();
    glPopMatrix();

    glLoadIdentity();
    glPushMatrix();

    // life symbol

    state.textures[T_HUD_1]->bind();

    glTranslatef(state.hud_x + .5f - state.tilt_x*.01f, state.hud_y - .7f - state.tilt_y*.01f, -9.9f);
    glColor4f(1.0f, .8f, .55f, alpha * .85f);
    glBegin (GL_QUADS);
      glTexCoord2i (0, 0);
      glVertex3f(-.17f, .17f, 0);

      glTexCoord2i(1, 0);
      glVertex3f(.17f, .17f, 0);

      glTexCoord2i(1, 1);
      glVertex3f(.17f, -.17f, 0);

      glTexCoord2i(0, 1);
      glVertex3f(-.17f, -.17f, 0);
    glEnd();

    // life bar

    s = int(50.0f / ((float(player->getLifeMaximum() + 1) / float(player->getLife() + 1))));

    for (i = 0; i < s; i++) {
        drawText("I", .32f + .077f * i, -.15f, 0, 80, 1.0f, .4f, .2f, .85f * (1.0f - (.02f * ((s + 1) - i))) * alpha);
    }

    // energy symbol

    state.textures[T_HUD_2]->bind();

    glTranslatef(0, -.375f, 0);
    glColor4f(1.0f, .8f, .55f, alpha * .85f);
    glBegin (GL_QUADS);
      glTexCoord2i (0, 0);
      glVertex3f(-.17f, .17f, 0);

      glTexCoord2i(1, 0);
      glVertex3f(.17f, .17f, 0);

      glTexCoord2i(1, 1);
      glVertex3f(.17f, -.17f, 0);

      glTexCoord2i(0, 1);
      glVertex3f(-.17f, -.17f, 0);
    glEnd();

    // energy bar

    e = int(50.0f / (((float)player->getEnergyMaximum() + 1) / (float(player->getEnergy() + 1))));

    for (i = 0; i < e; i++) {
        drawText("I", .32f + .077f * i, -.11f, 0, 80, .2f, .65f, 1.0f, .85f * (1.0f - (.02f * ((e + 1) - i))) * alpha);
    }

    glPopMatrix();
}

void Scene::drawMessages()
{
    int i;
    float r = 1.0f, g = .8f, b = .55f;
    float x, y, z;

    glLoadIdentity();
    glPushMatrix();
    glTranslatef(.0f, -1.75f, -10.0f);

    for (i=0; i<state.msg_num; i++) {

        switch(state.msg[i].type) {

            case MSG_DAMAGE:
                r = 1.0f;
                g = .25f + state.msg[i].counter * .0055f;
                b = .15f + state.msg[i].counter * .004f;
                break;

            case MSG_MONEY:
                r = 1.0f;
                g = 1.0f - state.msg[i].counter * .002f;
                b = .3f + state.msg[i].counter * .0025f;
                break;

            case MSG_ENERGY:
                r = .3f + state.msg[i].counter * .0025f;
                g = .75f - state.msg[i].counter * .002f;
                b = 1.0f;
                break;

            default:
                continue;

        }

        if (state.msg[i].direction_x > 0) {
            x = state.msg[i].direction_x * (state.msg[i].counter * state.msg[i].counter * state.vid_aspect * .0003f);
        } else {
            x = state.msg[i].direction_x * (state.msg[i].counter * state.msg[i].counter * state.vid_aspect * .0004f);
        }

        x += state.msg[i].direction_x * 1.0f;
        y = -1.5f + state.msg[i].direction_y * (state.msg[i].counter * .0265f);
        z = state.msg[i].counter * .01f;

        drawText(
            state.msg[i].text,
            x, y, z,
            75.0f + state.msg[i].counter * .75f,
            r, g, b,
            1.0f - state.msg[i].counter * .01f
        );
    }

    glPopMatrix();
}

/*
 * update game scene
 */
void Scene::updateScene()
{
    static GLuint nextdebris = state.timer;

    if ((state.timer > nextdebris) && (state.lvl_pos < float(state.lvl_length - 1000))) {
        nextdebris = state.timer + 250 + rand() % 250;
        state.entities.push_back(make_shared<Debris>());
    }

    state.player->resetTarget();

    auto e = state.entities.begin();

    while (e != state.entities.end()) {
        if ((*e)->isIdle()) {
            if ((*e)->getPosZ() < state.lvl_pos) {
                (*e)->setPosZ(-9999.0f);
                (*e)->activate();
            } else {
                ++e;
                continue;
            }
        }

        if ((*e)->isFocusable()) {
            state.player->checkTarget(state, *e);
        }

        (*e)->update(state);

        if ((*e)->isGone()) {
            e = state.entities.erase(e);
            continue;
        }

        if ((*e)->isCollider()) {
            auto f = next(e);

            while (f != state.entities.end()) {

                if (
                    (*f)->isCollider() &&
                    (*f)->isColliding(state, *e)
                ) {
                    (*e)->collide(state, *f);
                    (*f)->collide(state, *e);
                }

                ++f;
            }
        }

        ++e;
    }
}

/*
 * update money/damage messages
 */
void Scene::updateMessages()
{
    int i;

    for (i=0; i<state.msg_num; i++) {
        state.msg[i].counter += state.timer_adjustment * (.5f + state.msg[i].counter * .035f);

        if (state.msg[i].counter > 100.0f) {
            state.msg_num--;

            if (i < state.msg_num) {
                memcpy(&state.msg[i], &state.msg[state.msg_num], sizeof(Message));
                i--;
            }
        }
    }
}

/*
 * update everything
 */
void Scene::update()
{
    int i;

    skybox->update(state);

    switch (state.get()) {
        case STATE_MENU:
            if (state.menu_title_pos < 99.85f) {
                state.menu_title_pos += (100.1f - state.menu_title_pos) * state.timer_adjustment * .025f;
                state.global_alpha = (int)state.menu_title_pos;
            }
            player->setSpin(0, 0, 3.5f);
            player->setVelocity(0, 0, 0);
            player->setAcceleration(0, 0, 0);
            player->update(state);
            break;

        case STATE_QUIT:
            if (state.menu_title_pos > 0) {
                state.menu_title_pos -= (100.1f - state.menu_title_pos) * state.timer_adjustment * .15f;
                state.global_alpha = (int)state.menu_title_pos;
                player->update(state);
            } else {
                state.set(STATE_CLOSE);
            }
            break;

        case STATE_GAME_START:
            if (state.menu_title_pos > 0) {
                state.menu_title_pos -= (100.1f - state.menu_title_pos) * state.timer_adjustment * .15f;
                state.stars_rotation_speed = .05f * state.menu_title_pos * .01f;
            }

            if (state.stars_speed > .3f) {
                state.stars_speed -= (state.stars_speed - .2f) * .02f * state.timer_adjustment;
                player->update(state);
            } else if (state.lvl_loaded) {
                player->setPos(0, -90.0f, 50.0f);
                player->setRot(90.0f, 0, 270.0f);
                player->setSpin(0, 0, 0);

                state.set(STATE_GAME_LOOP);
            } else {
                state.set(STATE_QUIT);
            }
            break;

        case STATE_GAME_LOOP:
            state.lvl_pos += state.timer_adjustment * 1.5f;

            if (
                player->isAlive() &&
                state.lvl_pos > state.lvl_length
            ) {
                state.set(STATE_GAME_NEXTLEVEL);
            }

            updateScene();
            updateMessages();

            if (state.lvl_pos < 50) {
                player->setAccelerationZ(17.5f);
            } else {
                if (player->isAlive()) {
                    player->setAccelerationZ(0);
                } else {
                    player->setAccelerationZ(-4.0f);
                }
            }

            // update chase cam position

            if (player->isAlive()) {
                if (state.cam_speed < .5f) {
                    state.cam_speed += (.5f - state.cam_speed) * .01f * state.timer_adjustment;
                }
            } else {
                if (state.cam_speed > .01f) {
                    state.cam_speed -= (state.cam_speed + .015f) * .015f * state.timer_adjustment;
                } else {
                    state.cam_speed = 0;
                }
            }

            state.cam_x += state.timer_adjustment * ((player->getPosX() * state.cam_speed) - (state.cam_x * state.cam_speed)) * .15f;
            state.cam_y += state.timer_adjustment * ((player->getPosY() * state.cam_speed) - ((state.cam_y - state.cam_y_offset) * state.cam_speed)) * .175f;
            break;

        case STATE_GAME_NEXTLEVEL:
            updateScene();
            updateMessages();

            if (state.menu_title_pos < 99.85f) {
                state.menu_title_pos += state.timer_adjustment * .375;
                state.cam_y_offset = 35.0f + state.menu_title_pos * .85f;

                if (state.menu_title_pos > 80.0f) {
                    state.global_alpha = int(99.85f - (state.menu_title_pos - 80.0f) * 5.0f);
                    player->setAccelerationZ(350.0f);
                } else {
                    state.global_alpha = 100;
                    state.tilt(state.menu_title_pos * .05f);
                    player->setAccelerationZ(state.menu_title_pos * state.menu_title_pos * -.0001f);
                }

                if (state.stars_speed < 1.75f) {
                    state.stars_speed += (state.stars_speed - .1f) * .03f * state.timer_adjustment;
                }

                state.cam_x += state.timer_adjustment * ((player->getPosX() * state.cam_speed) - ((state.tilt_x * 2.0f + state.cam_x) * state.cam_speed)) * .15f;

                state.cam_y += state.timer_adjustment * ((player->getPosY() * state.cam_speed) - ((state.tilt_y * 2.0f + state.cam_y - state.cam_y_offset) * state.cam_speed)) * .15f;
            } else {
                player->setRot(115.0f, 0, 0);
                player->setVelocity(0, 0, 0);
                player->setAcceleration(0, 0, 0);
                state.set(STATE_MENU);
            }
            break;

        case STATE_GAME_QUIT:
            updateScene();
            updateMessages();

            if (state.menu_title_pos < 99.85f) {
                if (player->isAlive()) {
                    player->setAccelerationZ(-25.0f);
                }

                state.menu_title_pos += (100.1f - state.menu_title_pos) * state.timer_adjustment * .075f;
                state.global_alpha = (int)(100.1f - state.menu_title_pos);
            } else {
                player->setRot(115.0f, 0, 0);
                state.set(STATE_MENU);
            }
            break;
    }
}

/*
 * draw everything
 */
void Scene::draw()
{
    float p_x = .0f;
    float p_y = -90.0f;

    if (
        state.get() >  STATE_GAME_START &&
        state.get() <= STATE_GAME_QUIT
    ) {
        p_x = player->getPosX();
        p_y = player->getPosY();

        if (p_x < -600.0f) p_x = -600.0f;
        if (p_x >  600.0f) p_x =  600.0f;
        if (p_y < -400.0f) p_y = -400.0f;
        if (p_y >  400.0f) p_y =  400.0f;
    }

    glClearColor(0, 0, 0, 0);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, state.vid_width, state.vid_height);

    gluLookAt(
        p_x * -.01f + state.tilt_x * .4f,
        p_y * -.01f + player->getVelocityY() * 5.0f + state.tilt_y * .4f,
        200.0f,

        player->getVelocityX(),
        .0f,
        -10000.0f,

        player->getVelocityX() * .1f,
        -1.0f,
        .0f
    );

    state.view.lookAt(
        p_x * -.01f + state.tilt_x * .4f,
        p_y * -.01f + player->getVelocityY() * 5.0f + state.tilt_y * .4f,
        .0f,

        player->getVelocityX(),
        .0f,
        -10000.0f,

        player->getVelocityX() * .1f,
        -1.0f,
        .0f
    );

    // scenery

    skybox->draw(state);

    if (
        state.get() >= STATE_GAME_START &&
        state.get() <= STATE_GAME_QUIT
    ) {
        drawScene();
    }

    if (state.menu) {
        player->draw(state);
    }

    // menu

    glDisable(GL_DEPTH_TEST);

    if (
        state.get() >= STATE_GAME_START &&
        state.get() <= STATE_GAME_QUIT
    ) {
        drawMessages();
        drawDisplay();
    }

    if (state.menu) {
        drawMenu();
        drawTitle();
    }

    if (state.fps_visible) {
        drawVideoInfos();
    }

    glEnable(GL_DEPTH_TEST);
}
