#include "scene.hpp"

Scene::Scene(State &s) : state(s)
{
    player = make_shared<Player>();
    skybox = make_unique<Skybox>(state);
    overlay = make_unique<Overlay>(state);

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

    state.player.reset();
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

    // shaders

    state.log("Loading shader 'font'\n");
    state.shaders.insert(make_pair(S_FONT, new Shader(
        string(state.dir_resources).append("/shaders/font.glvs"),
        string(state.dir_resources).append("/shaders/font.glfs")
    )));

    state.log("Loading shader 'texture'\n");
    state.shaders.insert(make_pair(S_TEXTURE, new Shader(
        string(state.dir_resources).append("/shaders/texture.glvs"),
        string(state.dir_resources).append("/shaders/texture.glfs")
    )));

    state.log("Loading shader 'object'\n");
    state.shaders.insert(make_pair(S_OBJECT, new Shader(
        string(state.dir_resources).append("/shaders/object.glvs"),
        string(state.dir_resources).append("/shaders/object.glfs")
    )));

    // fonts

    state.log("Loading font 'zekton'\n");
    state.fonts.insert(make_pair(F_ZEKTON, new Font(
        string(state.dir_resources).append("/fonts/zekton.ttf"),
        make_shared<Shader>(*state.shaders[S_FONT]),
        state.vid_font_resolution
    )));

    // models

    state.log("Loading model 'ship_1'\n");
    state.models.insert(make_pair(OBJ_PLAYER, new Model(
        make_shared<Object>(string(state.dir_resources).append("/models/ship_1.obj")),
        make_shared<Texture>(string(state.dir_resources).append("/models/ship_1.tga")),
        make_shared<Shader>(*state.shaders[S_OBJECT])
    )));

    state.log("Loading model 'asteroid_1'\n");
    state.models.insert(make_pair(OBJ_ASTEROID_1, new Model(
        make_shared<Object>(string(state.dir_resources).append("/models/asteroid_1.obj")),
        make_shared<Texture>(string(state.dir_resources).append("/models/asteroid_1.tga")),
        make_shared<Shader>(*state.shaders[S_OBJECT])
    )));

    state.log("Loading model 'debris_1'\n");
    state.models.insert(make_pair(OBJ_DEBRIS_1, new Model(
        make_shared<Object>(string(state.dir_resources).append("/models/debris_1.obj")),
        make_shared<Texture>(string(state.dir_resources).append("/models/debris_1.tga")),
        make_shared<Shader>(*state.shaders[S_OBJECT])
    )));

    state.log("Loading model 'cargo_1'\n");
    state.models.insert(make_pair(OBJ_CARGO_1, new Model(
        make_shared<Object>(string(state.dir_resources).append("/models/cargo_1.obj")),
        make_shared<Texture>(string(state.dir_resources).append("/models/cargo_1.tga")),
        make_shared<Shader>(*state.shaders[S_OBJECT])
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

    state.entities.clear();
    state.spawns.clear();
    state.spawn(player);

    sprintf(msg, "Loading 'lvl/mission_%d.dat'... ", state.lvl_id);
    state.log(msg);

    sprintf(fname, "%s/levels/mission_%d.dat", state.dir_resources, state.lvl_id);
    fp = fopen(fname, "r");

    if (fp == NULL) {
        state.log("failed\n");
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

                                state.spawn(asteroid);
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

                                state.spawn(cargo);
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

                                state.spawn(asteroid);
                            }
                            break;
                    }
                }
            }
        }
    }
    
    state.log("ok\n");

    return true;
}

/*
 * update game scene
 */
void Scene::updateScene()
{
    static GLuint nextdebris = state.timer;

    if ((state.timer > nextdebris) && (state.lvl_pos < float(state.lvl_length - 1000))) {
        nextdebris = state.timer + 200 + rand() % 200;
        state.spawn(make_shared<Debris>());
    }

    player->resetTarget();

    auto s = state.spawns.begin();

    while (s != state.spawns.end()) {
        bool enter = true;

        if ((*s)->isIdle()) {
            enter = false;

            if ((*s)->getPosZ() < state.lvl_pos) {
                (*s)->setPosZ(-9999.0f);
                (*s)->activate();

                enter = true;
            }
        }

        if (enter) {
            state.entities.push_back(*s);
            s = state.spawns.erase(s);
            continue;
        }

        ++s;
    }

    auto e = state.entities.begin();

    while (e != state.entities.end()) {
        (*e)->update(state);

        if ((*e)->isGone()) {
            e = state.entities.erase(e);
            continue;
        }

        if ((*e)->isFocusable()) {
            player->checkTarget(state, *e);
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

    sort(state.entities.begin(), state.entities.end(), Entity::sort);
}

/*
 * update money/damage messages
 */
void Scene::updateMessages()
{
    auto m = state.messages.begin();

    while (m != state.messages.end()) {
        (*m)->counter += state.timer_adjustment * (.5f + (*m)->counter * .035f);
        ++m;
    }
}

/*
 * update everything
 */
void Scene::update()
{
    int i;

    skybox->update();

    switch (state.get()) {
        case STATE_MENU:
            if (state.menu_title_pos < 100.0f) {
                state.menu_title_pos += (100.1f - state.menu_title_pos) * state.timer_adjustment * .025f;
                state.global_alpha = (int)state.menu_title_pos;
            }

            state.cam_x = 0;
            state.cam_y = 0;
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
            if (state.lvl_loaded == false) {
                state.lvl_loaded = loadLevel();

                if (state.lvl_loaded == false) {
                    state.set(STATE_QUIT);
                } else {
                    player->init(state);
                }
            }

            if (state.menu_title_pos > 0) {
                state.menu_title_pos -= (100.1f - state.menu_title_pos) * state.timer_adjustment * .15f;
                state.stars_rotation_speed = .05f * state.menu_title_pos * .01f;
            }

            if (state.stars_speed > .3f) {
                state.stars_speed -= (state.stars_speed - .2f) * .02f * state.timer_adjustment;
            } else {
                state.set(STATE_GAME_LOOP);
            }
            break;

        case STATE_GAME_LOOP:
            state.lvl_pos += state.timer_adjustment * 1.5f;

            if (player->isAlive() &&
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

            if (state.menu_title_pos < 100.0f) {
                state.menu_title_pos += state.timer_adjustment * .375;
                state.cam_y_offset = 35.0f + state.menu_title_pos * .85f;

                if (state.menu_title_pos > 80.0f) {
                    state.global_alpha = int(100.0f - (state.menu_title_pos - 80.0f) * 5.0f);
                    player->setAccelerationZ(500.0f);
                } else {
                    state.global_alpha = 100;
                    state.tilt(state.menu_title_pos * .05f);
                    player->setAccelerationZ(state.menu_title_pos * state.menu_title_pos * -.0001f);
                }

                if (state.stars_speed < 1.75f) {
                    state.stars_speed += (state.stars_speed - .1f) * .03f * state.timer_adjustment;
                }

                state.cam_x += state.timer_adjustment * ((player->getPosX() * state.cam_speed) - (state.cam_x * state.cam_speed)) * .15f;
                state.cam_y += state.timer_adjustment * ((player->getPosY() * state.cam_speed) - ((state.cam_y - state.cam_y_offset) * state.cam_speed)) * .15f;
            } else {
                state.set(STATE_MENU);
            }
            break;

        case STATE_GAME_QUIT:
            updateScene();
            updateMessages();

            if (state.menu_title_pos < 100.0f) {
                if (player->isAlive()) {
                    player->setAccelerationZ(-25.0f);
                }

                state.menu_title_pos += (100.1f - state.menu_title_pos) * state.timer_adjustment * .075f;
                state.global_alpha = (int)(100.1f - state.menu_title_pos);
            } else {
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

    state.view->setCamera(
        p_x * -.01f + state.tilt_x * .333f,
        p_y * -.01f + state.tilt_y * .333f + player->getVelocityY() * 5.0f,
        0,

        0, 0, -10000.0f,

        player->getVelocityX() * .15f, -1.0f, 0
    );

    // background

    skybox->draw();

    // entities

    if (
        state.get() >= STATE_GAME_LOOP &&
        state.get() <= STATE_GAME_QUIT
    ) {
        auto e = state.entities.begin();

        while (e != state.entities.end()) {
            (*e)->draw(state);

            if ((*e)->isFocusable()) {
                (*e)->drawCrosshair(state, *e);
            }

            ++e;
        }
    }

    // overlay

    overlay->draw();
}
