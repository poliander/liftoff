#include "overlay.hpp"

Overlay::Overlay(State& s, shared_ptr<Player> p) : state(s), player(p)
{
    view = View::createPerspective(65.0f, 1.0f, .01f, 10000.0f);
    framebuffer = make_unique<Framebuffer>(s.vid_framebuffer_size, GL_RGB);
}

Overlay::~Overlay()
{
}

void Overlay::drawMessages()
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

        state.fonts[F_ZEKTON]->draw(
            state.msg[i].text,

            200.0f + x * 13.5f,
            150.0f + y * 32.0f,

            0.1f,
            r, g, b, 1.0f - state.msg[i].counter * .01f
        );
    }

    glPopMatrix();
}

/*
 * draw head-up-display (energy, shields, money)
 */
void Overlay::drawDisplay()
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
    glPopMatrix();

    // money

    sprintf(msg, "%d $", player->getMoney());
    state.fonts[F_ZEKTON]->draw(
        msg,

        114.0f + (state.hud_x - state.tilt_x * .01f) * 18.0f * -1.0f,
        127.5f + (state.hud_y - state.tilt_y * .01f) * 24.5f,

        0.15f,

        1.0f,
        1.0f,
        0.3f,

        alpha * .85f
    );

    // lower left screen

    state.textures[T_MENU_1]->bind();

    glLoadIdentity();
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
      glVertex3f(-.17f, -.17f, 0);

      glTexCoord2i(1, 0);
      glVertex3f(.17f, -.17f, 0);

      glTexCoord2i(1, 1);
      glVertex3f(.17f, .17f, 0);

      glTexCoord2i(0, 1);
      glVertex3f(-.17f, .17f, 0);
    glEnd();

    // life bar

    s = int(50.0f / ((float(player->getLifeMaximum() + 1) / float(player->getLife() + 1))));

    for (i = 0; i < s; i++) {
        state.fonts[F_ZEKTON]->draw(
            "I",

            213.0f - ((state.hud_x - state.tilt_x * .01f) * -18.0f) + 1.45f * i,
            134.5f + ((state.hud_y - state.tilt_y * .01f) *  24.5f),

            0.09f,

            1.0f,
            0.4f,
            0.2f,

            0.85f * (1.0f - (.02f * ((s + 1) - i))) * alpha
        );
    }

    // energy symbol

    state.textures[T_HUD_2]->bind();

    glTranslatef(0, -.375f, 0);
    glColor4f(1.0f, .8f, .55f, alpha * .85f);
    glBegin (GL_QUADS);
      glTexCoord2i (0, 0);
      glVertex3f(-.17f, -.17f, 0);

      glTexCoord2i(1, 0);
      glVertex3f(.17f, -.17f, 0);

      glTexCoord2i(1, 1);
      glVertex3f(.17f, .17f, 0);

      glTexCoord2i(0, 1);
      glVertex3f(-.17f, .17f, 0);
    glEnd();
    glPopMatrix();

    // energy bar

    e = int(50.0f / (((float)player->getEnergyMaximum() + 1) / (float(player->getEnergy() + 1))));

    for (i = 0; i < e; i++) {
        state.fonts[F_ZEKTON]->draw(
            "I",

            213.0f - ((state.hud_x - state.tilt_x * .01f) * -18.0f) + 1.45f * i,
            126.0f + ((state.hud_y - state.tilt_y * .01f) *  24.5f),

            0.09f,

            0.2f,
            0.65f,
            1.0f,

            0.85f * (1.0f - (.02f * ((s + 1) - i))) * alpha
        );
    }
}

/*
 * draw menu
 */
void Overlay::drawMenu()
{
    int i, numentries;
    float m_a = state.global_alpha;
    float s, x, y1, y2, a = state.menu_title_pos * .01f;

    float mfo; // font y-offset
    float mfs; // font size
    float mrh; // font row height

    char *mtxt[5];
    char msg[255];

    for (i=0; i<5; i++) {
        mtxt[i] = (char*)malloc(sizeof(char)*64);
    }

    switch (state.menu) {
        case 1: // main menu
            numentries = 3;

            mfo = 0.0f;
            mfs  = 0.15f;

            strcpy(mtxt[0], "LAUNCH");
            strcpy(mtxt[1], "SETTINGS");
            strcpy(mtxt[2], "QUIT");

            if (state.menu_selected) {
                switch (state.menu_pos) {
                    case 0: // launch
                        state.set(STATE_GAME_START);
                        player->init(state);
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

            mfo = 3.5f;
            mfs = 0.14f;

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
                        state.config.vid_quality = state.vid_quality;
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

            mfo = 9.0f;
            mfs = 0.075f;

            sprintf(
                mtxt[0],
                "SCREEN SIZE:\n     %dx%d",
                state.vid_cap_modes[state.vid_mode].w,
                state.vid_cap_modes[state.vid_mode].h
            );

            switch (state.config.vid_quality) {
                case 0:
                    strcpy(mtxt[1], "QUALITY:\n     VERY LOW");
                    break;

                case 1:
                    strcpy(mtxt[1], "QUALITY:\n     LOW");
                    break;

                case 2:
                    strcpy(mtxt[1], "QUALITY:\n     MEDIUM");
                    break;

                case 3:
                    strcpy(mtxt[1], "QUALITY:\n     HIGH");
                    break;

                case 4:
                    strcpy(mtxt[1], "QUALITY:\n     VERY HIGH");
                    break;

                case 5:
                    strcpy(mtxt[1], "QUALITY:\n     ULTRA");
                    break;
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

                    case 1: // toggle display quality
                        state.config.vid_quality++;

                        if (state.config.vid_quality > 5) {
                            state.config.vid_quality = 0;
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

            mfo = 8.0f;
            mfs = 0.075f;

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

    state.shaders[S_TEXTURE]->bind();
    state.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, m_a * .01f));
    state.shaders[S_TEXTURE]->update(UNI_MVP, state.orthographic->transform(
        0.0f,   -39.0f,
        350.0f, 180.0f
    ));
    state.textures[T_MENU_1]->bind();
    state.textures[T_MENU_1]->draw();
    state.shaders[S_TEXTURE]->unbind();

    state.fonts[F_ZEKTON]->draw(
        "VECTOR ZERO MK. IX \"REDUX\"",
        197.0f, 100.0f, 0.075f,
        1.0f, 0.9f, 0.85f, .0045f * m_a
    );

    // draw menu items

    mrh = 150.0f * (1.0f / float(numentries));

    state.shaders[S_TEXTURE]->bind();
    state.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(m_a * .005f, m_a * .005f, m_a * .005f, m_a * .0035f));
    state.shaders[S_TEXTURE]->update(UNI_MVP, state.orthographic->transform(
        -104.75f,
        36.5f - (mrh * float(state.menu_pos) + mrh * 0.5f),

        135.0f,
        mrh
    ));
    state.textures[T_MENU_2]->bind();
    state.textures[T_MENU_2]->draw();
    state.shaders[S_TEXTURE]->unbind();

    for (i = 0; i < numentries; i++) {
        float r, g, b;

        if (i == state.menu_pos) {
            r = 1.00f;
            g = 0.90f;
            b = 0.62f;
        } else {
            r = 1.00f;
            g = 0.80f;
            b = 0.55f;
        }

        state.fonts[F_ZEKTON]->draw(
            mtxt[i],

            122.0f,
            152.0f + mfo - (i * mrh * 0.5f),

            mfs,

            r, g, b,
            0.0085f * m_a
        );
    }

    if (state.get() == STATE_MENU) {
        x = 0;
        y1 = 400.0f - powf(state.menu_title_pos, 2) * .02f;
        y2 = 60.0f + powf(state.menu_title_pos, 2) * .01f;
        s = powf(state.menu_title_pos * .01f, 2) * 1.5f;
    } else {
        x = -400.0f + powf(state.menu_title_pos, 2) * .04f;
        y1 = 200.0f;
        y2 = 160.0f;
        s = 1.5f;
    }

    state.shaders[S_TEXTURE]->bind();
    state.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, a));

    // "LIFT-OFF"

    state.shaders[S_TEXTURE]->update(UNI_MVP, state.orthographic->transform(x, y1, 280.0f, 70.0f));

    state.textures[T_TITLE]->bind();
    state.textures[T_TITLE]->setTextureCoordinates(glm::vec4(0, 1.0f, 1.0f, .4f));
    state.textures[T_TITLE]->draw();

    // "BEYOND GLAXIUM"

    state.shaders[S_TEXTURE]->update(UNI_MVP, state.orthographic->transform(-x, y2, 280.0f * s, 40.0f * s));

    state.textures[T_TITLE]->setTextureCoordinates(glm::vec4(0, .4f, 1.0f, 0));
    state.textures[T_TITLE]->draw();

    state.shaders[S_TEXTURE]->unbind();

    glEnable(GL_DEPTH_TEST);
    player->draw(state);
    glDisable(GL_DEPTH_TEST);
}

/*
 * display video infos (FPS, MSAA level)
 */
void Overlay::drawVideoInfos()
{
    static char txt[16];

    // FPS

    sprintf(txt, "FPS: %d", int(round(state.fps)));
    state.fonts[F_ZEKTON]->draw(txt, 5.0f, 288.0f, 0.075f, 1.0f, 1.0f, 1.0f, .75f);

    // MSAA

    if (state.vid_multisampling > 0) {
        sprintf(txt, "MSAA: %dx", int(state.vid_multisampling));
    } else {
        sprintf(txt, "MSAA: OFF");
    }

    state.fonts[F_ZEKTON]->draw(txt, 5.0f, 282.0f, 0.075f, 1.0f, 1.0f, 1.0f, .75f);
}

void Overlay::draw()
{
    glDisable(GL_DEPTH_TEST);

    if (state.get() >= STATE_GAME_START &&
        state.get() <= STATE_GAME_QUIT
    ) {
        drawMessages();
        drawDisplay();
    }

    if ((state.get() >= STATE_MENU && state.get() <= STATE_GAME_START) ||
        (state.get() >= STATE_GAME_QUIT)
    ) {
        drawMenu();
    }

    if (state.fps_visible) {
        drawVideoInfos();
    }

    glEnable(GL_DEPTH_TEST);
}
