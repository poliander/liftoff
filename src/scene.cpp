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

#include "scene.hpp"

Scene::Scene(State* s) : state(s) {
    player = make_shared<Player>(state);
    skybox = make_unique<Skybox>(state);
    overlay = make_unique<Overlay>(state);

    state->player = player;

    load();
}

Scene::~Scene() {
    if (state->audio.volume_music) {
        Mix_HaltMusic();
        Mix_FreeMusic(state->audio.music[0]);
    }

    if (state->audio.volume_sfx) {
        Mix_FreeChunk(state->audio.sample[10]);
        Mix_FreeChunk(state->audio.sample[9]);
        Mix_FreeChunk(state->audio.sample[8]);
        Mix_FreeChunk(state->audio.sample[7]);
        Mix_FreeChunk(state->audio.sample[6]);
        Mix_FreeChunk(state->audio.sample[5]);
        Mix_FreeChunk(state->audio.sample[4]);
        Mix_FreeChunk(state->audio.sample[3]);
        Mix_FreeChunk(state->audio.sample[2]);
        Mix_FreeChunk(state->audio.sample[1]);
        Mix_FreeChunk(state->audio.sample[0]);
    }

    overlay.reset();
    skybox.reset();
    player.reset();

    state->fonts.clear();
    state->textures.clear();
}

void Scene::load() {
    state->log("Loading 'title.tga'\n");
    state->textures.insert(make_pair(T_TITLE, new Texture(
        string(state->dir_resources).append("/textures/title.tga")
    )));

    state->log("Loading 'menu_1.tga'\n");
    state->textures.insert(make_pair(T_MENU_1, new Texture(
        string(state->dir_resources).append("/textures/menu_1.tga")
    )));

    state->log("Loading 'menu_2.tga'\n");
    state->textures.insert(make_pair(T_MENU_2, new Texture(
        string(state->dir_resources).append("/textures/menu_2.tga")
    )));

    state->log("Loading 'hud_1.tga'\n");
    state->textures.insert(make_pair(T_HUD_1, new Texture(
        string(state->dir_resources).append("/textures/hud_1.tga")
    )));

    state->log("Loading 'hud_2.tga'\n");
    state->textures.insert(make_pair(T_HUD_2, new Texture(
        string(state->dir_resources).append("/textures/hud_2.tga")
    )));

    state->log("Loading 'star.tga'\n");
    state->textures.insert(make_pair(T_STAR, new Texture(
        string(state->dir_resources).append("/textures/star.tga")
    )));

    state->log("Loading 'missile_1.tga'\n");
    state->textures.insert(make_pair(T_MISSILE_1, new Texture(
        string(state->dir_resources).append("/textures/missile_1.tga")
    )));

    state->log("Loading 'explosion_1.tga'\n");
    state->textures.insert(make_pair(T_EXPLOSION_1, new Texture(
        string(state->dir_resources).append("/textures/explosion_1.tga")
    )));

    state->log("Loading 'explosion_2.tga'\n");
    state->textures.insert(make_pair(T_EXPLOSION_2, new Texture(
        string(state->dir_resources).append("/textures/explosion_2.tga")
    )));

    state->log("Loading 'jet_exhaust.tga'\n");
    state->textures.insert(make_pair(T_JET_EXHAUST, new Texture(
        string(state->dir_resources).append("/textures/jet_exhaust.tga")
    )));

    state->log("Loading 'background_1.tga'\n");
    state->textures.insert(make_pair(T_BACKGROUND_1, new Texture(
        string(state->dir_resources).append("/textures/background_1.tga")
    )));

    state->log("Loading 'glow.tga'\n");
    state->textures.insert(make_pair(T_GLOW, new Texture(
        string(state->dir_resources).append("/textures/glow.tga")
    )));

    state->log("Loading 'halo.tga'\n");
    state->textures.insert(make_pair(T_HALO, new Texture(
        string(state->dir_resources).append("/textures/halo.tga")
    )));

    // shaders

    state->log("Loading shader 'font'\n");
    state->shaders.insert(make_pair(S_FONT, new Shader(
        string(state->dir_resources).append("/shaders/font.vert.glsl"),
        string(state->dir_resources).append("/shaders/font.frag.glsl")
    )));

    state->log("Loading shader 'texture'\n");
    state->shaders.insert(make_pair(S_TEXTURE, new Shader(
        string(state->dir_resources).append("/shaders/texture.vert.glsl"),
        string(state->dir_resources).append("/shaders/texture.frag.glsl")
    )));

    state->log("Loading shader 'object'\n");
    state->shaders.insert(make_pair(S_OBJECT, new Shader(
        string(state->dir_resources).append("/shaders/object.vert.glsl"),
        string(state->dir_resources).append("/shaders/object.frag.glsl")
    )));

    // fonts

    state->log("Loading font 'zekton'\n");
    state->fonts.insert(make_pair(F_ZEKTON, new Font(
        string(state->dir_resources).append("/fonts/zekton.ttf"),
        make_shared<Shader>(*state->shaders[S_FONT]),
        state->vid_font_resolution
    )));

    // models

    state->log("Loading model 'ship_1'\n");
    state->models.insert(make_pair(OBJ_PLAYER, new Model(
        make_shared<Object>(string(state->dir_resources).append("/models/ship_1.obj")),
        make_shared<Texture>(string(state->dir_resources).append("/models/ship_1.tga")),
        make_shared<Shader>(*state->shaders[S_OBJECT])
    )));

    state->log("Loading model 'asteroid_1'\n");
    state->models.insert(make_pair(OBJ_ASTEROID_1, new Model(
        make_shared<Object>(string(state->dir_resources).append("/models/asteroid_1.obj")),
        make_shared<Texture>(string(state->dir_resources).append("/models/asteroid_1.tga")),
        make_shared<Shader>(*state->shaders[S_OBJECT])
    )));

    state->log("Loading model 'debris_1'\n");
    state->models.insert(make_pair(OBJ_DEBRIS_1, new Model(
        make_shared<Object>(string(state->dir_resources).append("/models/debris_1.obj")),
        make_shared<Texture>(string(state->dir_resources).append("/models/debris_1.tga")),
        make_shared<Shader>(*state->shaders[S_OBJECT])
    )));

    state->log("Loading model 'cargo_1'\n");
    state->models.insert(make_pair(OBJ_CARGO_1, new Model(
        make_shared<Object>(string(state->dir_resources).append("/models/cargo_1.obj")),
        make_shared<Texture>(string(state->dir_resources).append("/models/cargo_1.tga")),
        make_shared<Shader>(*state->shaders[S_OBJECT])
    )));

    // sound effects

    state->audio.sample[0]  = state->audio.loadSample("menu_1.wav");
    state->audio.sample[1]  = state->audio.loadSample("menu_2.wav");
    state->audio.sample[2]  = state->audio.loadSample("gun_1.wav");
    state->audio.sample[3]  = state->audio.loadSample("engine_1.wav");
    state->audio.sample[4]  = state->audio.loadSample("explosion_1.wav");
    state->audio.sample[5]  = state->audio.loadSample("explosion_2.wav");
    state->audio.sample[6]  = state->audio.loadSample("explosion_3.wav");
    state->audio.sample[7]  = state->audio.loadSample("shield_1.wav");
    state->audio.sample[8]  = state->audio.loadSample("engine_2.wav");
    state->audio.sample[9]  = state->audio.loadSample("powerup_1.wav");
    state->audio.sample[10] = state->audio.loadSample("engine_3.wav");
    state->audio.sample[11] = state->audio.loadSample("logo.wav");

    // music

    state->audio.music[0]   = state->audio.loadMusic("music_title.ogg");
}

bool Scene::loadLevel() {
    FILE *fp = NULL;
    char msg[255], fname[255], buf[1024], cmd[16], par[255];

    int i, money, life, m, p = 0;
    uint16_t e_obj;
    float p_x, p_y, p_z;
    float r_x, r_y, r_z;
    float s_x, s_y, s_z;
    float w_x, w_y, w_z;

    state->entities.clear();
    state->spawns.clear();
    state->spawn(player);

    snprintf(msg, sizeof(msg), "Loading 'lvl/mission_1.dat'... ");
    state->log(msg);

    snprintf(fname, sizeof(msg), "%s/levels/mission_1.dat", state->dir_resources);
    fp = fopen(fname, "r");

    if (fp == NULL) {
        state->log("failed\n");
        return false;
    }

    while (!feof(fp)) {
        if (fgets(buf, 1024, fp) != NULL) {
            snprintf(cmd, sizeof(cmd), "");
            snprintf(cmd, sizeof(par), "");

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
                if (!strcmp(cmd, "soundtrack")) {
                    snprintf(state->lvl_music, sizeof(state->lvl_music), "%s", par);
                }

                // length
                if (!strcmp(cmd, "length")) sscanf(par, "%d", &state->lvl_length);

                // colliding object, obstacle
                if (!strcmp(cmd, "collider")) {
                    r_x = static_cast<float>(rand() % 3600) * .1f;
                    r_y = static_cast<float>(rand() % 3600) * .1f;
                    r_z = static_cast<float>(rand() % 3600) * .1f;

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
                                auto asteroid = make_shared<Asteroid>(state);

                                asteroid->setPos(p_x, p_y, p_z);
                                asteroid->setRot(r_x, r_y, r_z);
                                asteroid->setScale(s_x, s_y, s_z);
                                asteroid->setSpin(w_x, w_y, w_z);
                                asteroid->setLife(life);

                                state->spawn(asteroid);
                            }
                            break;

                        case OBJ_CARGO_1:
                            {
                                auto cargo = make_shared<Cargo>(state);

                                cargo->setPos(p_x, p_y, p_z);
                                cargo->setRot(r_x, r_y, r_z);
                                cargo->setScale(s_x, s_y, s_z);
                                cargo->setSpin(w_x, w_y, w_z);
                                cargo->setLife(life);

                                state->spawn(cargo);
                            }
                            break;
                    }
                }

                // scenery object
                if (!strcmp(cmd, "scenery")) {
                    r_x = static_cast<float>(rand() % 360);
                    r_y = static_cast<float>(rand() % 360);
                    r_z = static_cast<float>(rand() % 360);

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
                                auto asteroid = make_shared<Asteroid>(state);

                                asteroid->setType(E_TYPE_SCENERY);
                                asteroid->setPos(p_x, p_y, p_z);
                                asteroid->setRot(r_x, r_y, r_z);
                                asteroid->setScale(s_x, s_y, s_z);
                                asteroid->setSpin(w_x, w_y, w_z);
                                asteroid->setLife(life);

                                state->spawn(asteroid);
                            }
                            break;
                    }
                }
            }
        }
    }

    state->log("ok\n");

    return true;
}

void Scene::updateScene() {
    static GLuint nextdebris = SDL_GetTicks();

    if (SDL_GetTicks() > nextdebris &&
        state->lvl_pos < static_cast<float>(state->lvl_length - 1000)
    ) {
        nextdebris = SDL_GetTicks() + 200 + rand() % 200;
        state->spawn(make_shared<Debris>(state));
    }

    player->resetTarget();

    auto s = state->spawns.begin();

    while (s != state->spawns.end()) {
        bool enter = true;

        if ((*s)->isIdle()) {
            enter = false;

            if ((*s)->getPosZ() < state->lvl_pos) {
                (*s)->setPosZ(-9999.0f);
                (*s)->activate();

                enter = true;
            }
        }

        if (enter) {
            state->entities.push_back(*s);
            s = state->spawns.erase(s);
            continue;
        }

        ++s;
    }

    auto e = state->entities.begin();

    while (e != state->entities.end()) {
        (*e)->update();

        if ((*e)->isGone()) {
            e = state->entities.erase(e);
            continue;
        }

        if ((*e)->isFocusable()) {
            player->checkTarget(*e);
        }

        if ((*e)->isCollider()) {
            auto f = next(e);

            while (f != state->entities.end()) {
                if (
                    (*f)->isCollider() &&
                    (*f)->isColliding(*e)
                ) {
                    (*e)->collide(*f);
                    (*f)->collide(*e);
                }

                ++f;
            }
        }

        ++e;
    }

    sort(state->entities.begin(), state->entities.end(), Entity::sort);
}

void Scene::update() {
    skybox->update();
    overlay->update();

    switch (state->get()) {
        case STATE_MENU:
            state->global_alpha = state->global_transition;
            break;

        case STATE_GAME_START:
            if (state->lvl_loaded == false) {
                state->lvl_loaded = loadLevel();

                if (state->lvl_loaded == false) {
                    state->set(STATE_QUIT);
                } else {
                    player->init();
                }
            }

            if (state->global_transition < 1.0f) {
                state->stars_speed = 1.75f - state->global_transition * 1.4f;
                state->stars_rotation_speed = 0.05f - state->global_transition * 0.05f;
            } else {
                state->set(STATE_GAME_LOOP);
            }
            break;

        case STATE_GAME_LOOP:
            state->lvl_pos += state->global_timer * 1.5f;

            if (player->isAlive() &&
                state->lvl_pos > state->lvl_length
            ) {
                state->set(STATE_GAME_NEXTLEVEL);
            }

            updateScene();

            if (player->getPosZ() > -225.0f) {
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
                if (state->cam_speed < .5f) {
                    state->cam_speed += (.5f - state->cam_speed) * .01f * state->global_timer;
                }
            } else {
                if (state->cam_speed > .01f) {
                    state->cam_speed -= (state->cam_speed + .015f) * .015f * state->global_timer;
                } else {
                    state->cam_speed = 0;
                }
            }

            state->cam_x += state->global_timer * ((player->getPosX() * state->cam_speed) - (state->cam_x * state->cam_speed)) * .15f;
            state->cam_y += state->global_timer * ((player->getPosY() * state->cam_speed) - ((state->cam_y - state->cam_y_offset) * state->cam_speed)) * .175f;
            break;

        case STATE_GAME_NEXTLEVEL:
            updateScene();

            if (state->global_transition >= 1.0f) {
                state->set(STATE_MENU);
            }
            break;

        case STATE_GAME_QUIT:
            updateScene();

            if (state->global_transition < 1.0f) {
                if (player->isAlive()) {
                    player->setAccelerationZ(-25.0f);
                }

                state->global_alpha = 1.0f - state->global_transition;
            } else {
                state->set(STATE_MENU);
            }
            break;

        case STATE_QUIT:
            state->global_alpha = 1.0f - state->global_transition;

            if (state->global_transition < 1.0f) {
                player->update();
            } else {
                state->set(STATE_CLOSE);
            }
            break;
    }
}

void Scene::draw(const unique_ptr<Renderbuffer> &buffer) {
    float p_x = 0;
    float p_y = -90.0f;

    if (state->get() >= STATE_GAME_LOOP &&
        state->get() <= STATE_GAME_QUIT
    ) {
        p_x = player->getPosX();
        p_y = player->getPosY();

        if (p_x < -600.0f) p_x = -600.0f;
        if (p_x >  600.0f) p_x =  600.0f;
        if (p_y < -400.0f) p_y = -400.0f;
        if (p_y >  400.0f) p_y =  400.0f;
    }

    state->view->setCamera(
        p_x * -.01f,
        p_y * -.01f + player->getVelocityY() * 5.0f,
        0,

        0, 0, -1.0f,

        player->getVelocityX() * .15f, -1.0f, 0
    );

    // background

    skybox->draw();

    // entities

    if (state->get() >= STATE_GAME_LOOP &&
        state->get() <= STATE_GAME_QUIT
    ) {
        buffer->bind();
        buffer->clear();

        auto e = state->entities.begin();

        while (e != state->entities.end()) {
            (*e)->draw();

            if ((*e)->isFocusable()) {
                (*e)->drawCrosshair(*e);
            }

            ++e;
        }

        buffer->blit();
        buffer->unbind();
        buffer->draw();
    }

    // overlay

    overlay->draw();
}
