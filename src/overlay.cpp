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

#include "overlay.hpp"

Overlay::Overlay(State* s) : state(s) {
    buffer = make_unique<Renderbuffer>(state);

    perspective = View::createPerspective(65, 4.0f / 3.0f, .1f, 100.0f);
    perspective->setCamera(
        0, 0, 0,
        0, 0, -1.0f,
        0, 1.0f, -0
    );

    view = View::createOrthographic(-400.0f, 400.0f, -300.0f, 300.0f);
}

void Overlay::update() {
    auto m = state->messages.begin();

    while (m != state->messages.end()) {
        (*m)->counter += state->global_timer * (.5f + (*m)->counter * .035f);
        ++m;
    }

    float t = state->global_transition;

    switch (state->get()) {
        case STATE_MENU:
            status_visible      = false;
            menu_visible        = true;
            menu_cursor_visible = true;
            menu_alpha          = 1.0f;
            screen_y            = 50.0f;
            ship_y              = -3.0f;
            logo1_x             = 0;
            logo1_y             = ease_stop3(t) * 200.0f - 400.0f;
            logo2_x             = 0;
            logo2_y             = -150.0f;
            break;

        case STATE_GAME_START:
            status_visible      = true;
            menu_visible        = true;
            menu_cursor_visible = false;
            menu_alpha          = 1.0f - ease_stop2(t);
            status_alpha        = ease_start2(t);
            screen_y            = ease_step3(t) * 290.0f + 50.0f;
            ship_y              = ease_step3(t) * -25.0f - 3.0f;
            logo1_x             = ease_start3(t) * 500.0f * -1.0f;
            logo2_x             = ease_start3(t) * 500.0f;
            break;

        case STATE_GAME_LOOP:
            screen_y            = 340.0f;
            menu_visible        = false;
            status_visible      = true;
            status_alpha        = 1.0f;
            break;

        case STATE_GAME_QUIT:
            screen_y            = ease_start2(t) * 200.0f + 340.0f;
            break;

        case STATE_QUIT:
            logo1_x             = ease_start3(t) * 500.0f * -1.0f;
            logo2_x             = ease_start3(t) * 500.0f;
            break;
    }
}

void Overlay::drawMessages() {
    float x, y, a, s;
    auto m = state->messages.begin();

    while (m != state->messages.end()) {
        a = 1.0f - (*m)->counter * .01f;

        if ((*m)->dir_x > 0) {
            x = (*m)->dir_x * ((*m)->counter + 1.0f) * 1.4f;
        } else {
            x = (*m)->dir_x * ((*m)->counter + 1.0f) * 2.2f;
        }

        y = (*m)->dir_y * 10.0f * (((*m)->counter + 1.0f) * .05f * ((*m)->counter + 1.0f) * .05f);
        s = (*m)->counter * .0025f;

        state->fonts[F_ZEKTON]->draw(
            (*m)->text,
            x, 25.0f + y,
            s,
            (*m)->c_r, (*m)->c_g, (*m)->c_b, a
        );

        if (a < 0) {
            if ((*m)->type == MSG_MONEY) state->player->setMoney(state->player->getMoney() + (*m)->value);
            m = state->messages.erase(m);
            continue;
        }

        ++m;
    }
}

void Overlay::drawStatus() {
    char msg[16];

    // money

    snprintf(msg, sizeof(msg), "%d $", state->player->getMoney());
    state->fonts[F_ZEKTON]->draw(
        msg,
        -228.0f, screen_y - 52.0f,
        0.2f,
        1.0f, 1.0f, 0.3f, 1.0f * status_alpha
    );

    // life bar

    int s = static_cast<int>(80.0f / ((static_cast<float>(state->player->getLifeMaximum() + 1) / static_cast<float>(state->player->getLife() + 1))));

    for (int i = 0; i < s; i++) {
        state->fonts[F_ZEKTON]->draw(
            "I",
            -95.0f + i * 4.0f, screen_y - 65.0f,
            0.1f,
            1.0f, 0.4f, 0.2f, 0.85f * status_alpha
        );
    }

    // energy bar

    int e = static_cast<int>(80.0f / ((static_cast<float>(state->player->getEnergyMaximum() + 1) / static_cast<float>(state->player->getEnergy() + 1))));

    for (int i = 0; i < e; i++) {
        state->fonts[F_ZEKTON]->draw(
            "I",
            -95.0f + i * 4.0f, screen_y - 50.0f,
            0.1f,
            0.2f, 0.65f, 1.0f, 0.85f * status_alpha
        );
    }
}

void Overlay::drawScreen() {
    state->shaders[S_TEXTURE]->bind();

    state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    state->shaders[S_TEXTURE]->update(UNI_MVP, view->transform(
        0.0f,   screen_y,
        500.0f, 180.0f
    ));

    state->textures[T_MENU_1]->bind();
    state->textures[T_MENU_1]->draw();
}

void Overlay::drawMenu() {
    int numentries;
    float s;

    float mfo; // font y-offset
    float mfs; // font size
    float mrh; // font row height

    char mtxt[5][255];
    char msg[255];

    switch (state->menu) {
        case 1: // main menu
            numentries = 3;

            mfo = 0.0f;
            mfs  = 0.14f;

            snprintf(mtxt[0], sizeof(mtxt[0]), "LAUNCH");
            snprintf(mtxt[1], sizeof(mtxt[1]), "SETTINGS");
            snprintf(mtxt[2], sizeof(mtxt[3]), "QUIT");

            if (state->menu_selected) {
                switch (state->menu_pos) {
                    case 0: // launch
                        state->set(STATE_GAME_START);
                        break;

                    case 1: // enter settings
                        if (state->menu != 2) {
                            state->menu = 2;
                            state->menu_pos = 0;
                            state->menu_selected = false;
                            drawMenu();
                            return;
                        }
                        break;

                    case 2: // quit game
                        state->set(STATE_QUIT);
                        break;
                }
            }
            break;

        case 2: // settings submenu
            numentries = 4;

            mfo = -5.75f;
            mfs = 0.14f;

            snprintf(mtxt[0], sizeof(mtxt[0]), "VIDEO");
            snprintf(mtxt[1], sizeof(mtxt[1]), "AUDIO");
            snprintf(mtxt[2], sizeof(mtxt[2]), "CANCEL");
            snprintf(mtxt[3], sizeof(mtxt[3]), "ACCEPT");

            if (state->menu_selected) {
                switch (state->menu_pos) {
                    case 0: // enter video settings
                        state->menu = 3;
                        state->menu_pos = 0;
                        break;

                    case 1: // enter audio settings
                        state->menu = 4;
                        state->menu_pos = 0;
                        break;

                    case 2: // cancel
                        state->menu = 1;
                        state->menu_pos = 1;
                        state->config.aud_sfx = state->audio.volume_sfx;
                        state->config.aud_music = state->audio.volume_music;
                        state->config.aud_mixfreq = state->audio.mixer_frequency;
                        for (auto i = state->vid_modes.begin(); i != state->vid_modes.end(); i++) {
                            if (state->vid_width  == i->second.w &&
                                state->vid_height == i->second.h
                            ) {
                                state->vid_mode = i->first;
                            }
                        }
                        state->config.vid_quality = state->vid_quality;
                        state->config.vid_fullscreen = state->vid_fullscreen;
                        state->config.vid_vsync = state->vid_vsync;
                        break;

                    case 3: // accept
                        state->config.vid_width  = state->vid_modes[state->vid_mode].w;
                        state->config.vid_height = state->vid_modes[state->vid_mode].h;
                        state->set(STATE_RESTART);
                        break;
                }
            }

            break;

        case 3: // video settings
            numentries = 5;

            mfo = -18.25f;
            mfs = 0.075f;

            snprintf(
                mtxt[0],
                sizeof(mtxt[0]),
                "SCREEN SIZE:\n     %dx%d",
                state->vid_modes[state->vid_mode].w,
                state->vid_modes[state->vid_mode].h
            );

            switch (state->config.vid_quality) {
                case 0:
                    snprintf(mtxt[1], sizeof(mtxt[1]), "QUALITY:\n     VERY LOW");
                    break;

                case 1:
                    snprintf(mtxt[1], sizeof(mtxt[1]), "QUALITY:\n     LOW");
                    break;

                case 2:
                    snprintf(mtxt[1], sizeof(mtxt[1]), "QUALITY:\n     MEDIUM");
                    break;

                case 3:
                    snprintf(mtxt[1], sizeof(mtxt[1]), "QUALITY:\n     HIGH");
                    break;

                case 4:
                    snprintf(mtxt[1], sizeof(mtxt[1]), "QUALITY:\n     VERY HIGH");
                    break;

                case 5:
                    snprintf(mtxt[1], sizeof(mtxt[1]), "QUALITY:\n     ULTRA");
                    break;
            }

            if (state->config.vid_fullscreen) {
                snprintf(mtxt[2], sizeof(mtxt[2]), "FULL SCREEN:\n     ENABLED");
            } else {
                snprintf(mtxt[2], sizeof(mtxt[2]), "FULL SCREEN:\n     DISABLED");
            }

            if (state->config.vid_vsync) {
                snprintf(mtxt[3], sizeof(mtxt[3]), "VERTICAL SYNC:\n     ENABLED");
            } else {
                snprintf(mtxt[3], sizeof(mtxt[3]), "VERTICAL SYNC:\n     DISABLED");
            }

            snprintf(mtxt[4], sizeof(mtxt[4]), "RETURN");

            if (state->menu_selected) {
                switch (state->menu_pos) {
                    case 0: // toggle video mode
                        state->vid_mode--;

                        if (state->vid_mode < 0) {
                            state->vid_mode = state->vid_modes.size() - 1;
                        }
                        break;

                    case 1: // toggle display quality
                        state->config.vid_quality++;

                        if (state->config.vid_quality > 5) {
                            state->config.vid_quality = 0;
                        }
                        break;

                    case 2: // toggle fullscreen on/off
                        state->config.vid_fullscreen = !state->config.vid_fullscreen;
                        break;

                    case 3: // toggle vsync on/off
                        state->config.vid_vsync = !state->config.vid_vsync;
                        break;

                    case 4: // return
                        state->menu_pos = 0;
                        state->menu = 2;
                        break;
                }
            }
            break;

        case 4: // audio settings
            numentries = 4;

            mfo = -13.75f;
            mfs = 0.075f;

            switch(state->config.aud_sfx) {
                case 0:
                    snprintf(mtxt[0], sizeof(mtxt[0]), "SOUND FX:\n     MUTED");
                    break;

                case 1:
                    snprintf(mtxt[0], sizeof(mtxt[0]), "SOUND FX:\n     MINIMUM");
                    break;

                case 2:
                    snprintf(mtxt[0], sizeof(mtxt[0]), "SOUND FX:\n     MEDIUM");
                    break;

                case 3:
                    snprintf(mtxt[0], sizeof(mtxt[0]), "SOUND FX:\n     MAXIMUM");
                    break;

                default:
                    snprintf(mtxt[0], sizeof(mtxt[0]), "SOUND FX:\n     DISABLED");
            }

            switch(state->config.aud_music) {
                case 0:
                    snprintf(mtxt[1], sizeof(mtxt[1]), "MUSIC:\n     MUTED");
                    break;

                case 1:
                    snprintf(mtxt[1], sizeof(mtxt[1]), "MUSIC:\n     MINIMUM");
                    break;

                case 2:
                    snprintf(mtxt[1], sizeof(mtxt[1]), "MUSIC:\n     MEDIUM");
                    break;

                case 3:
                    snprintf(mtxt[1], sizeof(mtxt[1]), "MUSIC:\n     MAXIMUM");
                    break;

                default:
                    snprintf(mtxt[1], sizeof(mtxt[1]), "MUSIC:\n     DISABLED");
            }

            snprintf(mtxt[2], sizeof(mtxt[2]), "MIXER QUALITY:\n     %d HZ", state->config.aud_mixfreq);
            snprintf(mtxt[3], sizeof(mtxt[3]), "RETURN");

            if (state->menu_selected) {
                switch (state->menu_pos) {
                    case 0: // toggle SFX volume
                        if (state->config.aud_sfx != -1) {
                            state->config.aud_sfx++;

                            if (state->config.aud_sfx > 3) {
                                state->config.aud_sfx = 0;
                            }
                        }
                        break;

                    case 1: // toggle music volume
                        if (state->config.aud_music != -1) {
                            state->config.aud_music++;

                            if (state->config.aud_music > 3) {
                                state->config.aud_music = 0;
                            }
                        }
                        break;

                    case 2: // toggle mixer frequency
                        if (state->config.aud_mixfreq == 22050) {
                            state->config.aud_mixfreq = 44100;
                        } else {
                            state->config.aud_mixfreq = 22050;
                        }
                        break;

                    case 3: // return
                        state->menu = 2;
                        state->menu_pos = 1;
                        break;
                }
            }

            break;

        default:
            return;
    }

    if (state->menu_selected) {
        state->menu_selected = false;
        drawMenu();
        return;
    }

    if (state->menu_pos >= numentries) {
        state->menu_pos = numentries-1;
    }

    if (state->menu_pos < 0) {
        state->menu_pos = 0;
    }

    mrh = 150.0f * (1.0f / static_cast<float>(numentries));

    // menu cursor

    if (menu_cursor_visible) {
        state->shaders[S_TEXTURE]->bind();
        state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.5f, .5f, .5f, .35f * menu_alpha));
        state->shaders[S_TEXTURE]->update(UNI_MVP, view->transform(
            -146.5f, -25.0f + (mrh * static_cast<float>(state->menu_pos) + mrh * 0.5f),
            200.0f, mrh
        ));

        state->textures[T_MENU_2]->bind();
        state->textures[T_MENU_2]->draw();
    }

    // menu items

    for (int i = 0; i < numentries; i++) {
        float r, g, b;

        if (i == state->menu_pos) {
            r = 1.00f;
            g = 0.90f;
            b = 0.62f;
        } else {
            r = 1.00f;
            g = 0.80f;
            b = 0.55f;
        }

        state->fonts[F_ZEKTON]->draw(
            mtxt[i],

            -220.0f,
            -43.0f + screen_y + mfo + (i * mrh),

            mfs,

            r, g, b, .85f * menu_alpha
        );
    }

    if (state->get() == STATE_MENU) {
        s = 1.5f * ease_stop2(state->global_transition);
    } else {
        s = 1.5f;
    }

    state->shaders[S_TEXTURE]->bind();
    state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, menu_alpha));

    state->textures[T_TITLE]->bind();

    // "LIFT-OFF"

    state->shaders[S_TEXTURE]->update(UNI_MVP, view->transform(logo1_x, logo1_y, 400.0f, -80.0f));
    state->textures[T_TITLE]->setTextureCoordinates(glm::vec4(0, 1.0f, 1.0f, .4f));
    state->textures[T_TITLE]->draw();

    // "BEYOND GLAXIUM"

    state->shaders[S_TEXTURE]->update(UNI_MVP, view->transform(logo2_x, logo2_y, 400.0f * s, -50.0f * s));
    state->textures[T_TITLE]->setTextureCoordinates(glm::vec4(0, .4f, 1.0f, 0));
    state->textures[T_TITLE]->draw();

    state->shaders[S_TEXTURE]->unbind();

    // Caption

    state->fonts[F_ZEKTON]->draw(
        "VECTOR ZERO MK. IX \"REDUX\"",
        15.0f, screen_y + 70.0f, 0.075f,
        1.0f, 0.9f, 0.85f, .45f * menu_alpha
    );
}

void Overlay::drawShip() {
    static float rotation = 0;

    rotation -= state->global_timer * 0.3f;
    if (rotation < 0) rotation += 360.0f;

    glEnable(GL_DEPTH_TEST);

    glm::mat4 model = perspective->getModel(
        -7.5f, ship_y, -35.0f,
        -85.0f, 180.0f, rotation,
        1.5f, 1.5f, 1.5f
    );

    state->models[OBJ_PLAYER]->draw(
        model,
        perspective->getCamera(),
        perspective->getProjection(),
        glm::vec4(1.0f, 1.0f, 1.0f, menu_alpha)
    );

    glDisable(GL_DEPTH_TEST);
}

void Overlay::draw() {
    float alpha = state->global_alpha;

    glDisable(GL_DEPTH_TEST);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    buffer->bind();
    buffer->clear();

    if (menu_visible) {
        drawShip();
    }

    drawScreen();

    if (menu_visible) {
        drawMenu();
    }

    if (status_visible) {
        drawMessages();
        drawStatus();
    }

    if (state->fps_visible) {
        static char txt[16];

        snprintf(txt, sizeof(txt), "%.01f FPS", state->fps);
        state->fonts[F_ZEKTON]->draw(txt, -40.0f, -270.0f, 0.12f, 1.0f, 1.0f, 1.0f, .75f);
    }

    buffer->blit();
    buffer->unbind();
    buffer->draw(
        view->transform(0, 0, 1000.0f / state->vid_aspect, 600.0f),
        glm::vec4(alpha, alpha, alpha, alpha)
    );

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}
