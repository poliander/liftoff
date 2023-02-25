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

#include "engine.hpp"

bool Engine::init(int argc, char **argv) {
    char msg[255];
    SDL_DisplayMode mode, current;
    int flags;

    if (argc >= 0) {
        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], "/d") == 0 ||
                strcmp(argv[i], "-d") == 0
            ) {
                state.log_file = true;
                continue;
            }
        }
    }

    // load configuration

    state.log("Loading configuration... ");

    if (Configuration::load(state.dir_configuration, &state.config)) {
        state.log("ok\n");
    } else {
        state.log("failed (using defaults)\n");
    }

    // initialize SDL

    state.log("Initializing SDL... ");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER | SDL_INIT_EVENTS)) {
        snprintf(msg, sizeof(msg), "failed (%s)\n", SDL_GetError());
        state.log(msg);

        return false;
    } else {
        state.log("ok\n");
    }

    // initialize joystick/gamepad

    state.log("Initializing joystick/gamepad... ");

    if (SDL_NumJoysticks() > 0) {
        state.joystick = SDL_JoystickOpen(0);

        if (state.joystick) {
            state.log("ok\n");
        } else {
            state.log("failed\n");
        }
    } else {
        state.log("none found\n");
    }

    // look for valid display modes

    state.log("Initializing display...");

    for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i) {
        if (0 == SDL_GetCurrentDisplayMode(i, &current)) {
            if (state.vid_display == -1) {
                state.vid_display = i;
                state.vid_format = current.format;
                state.vid_refresh_rate = current.refresh_rate;
                state.vid_width = current.w;
                state.vid_height = current.h;
                break;
            }
        }
    }

    if (state.vid_display == -1) {
        state.log(" failed (no display found)\n");
        return false;
    }

    state.log("\n");

    for (int m = 0, i = 0; i < SDL_GetNumDisplayModes(state.vid_display); i++) {
        SDL_GetDisplayMode(state.vid_display, i, &mode);

        if (mode.format != state.vid_format) {
            continue;
        }

        snprintf(msg, sizeof(msg), "- found mode %d x %d @ %d Hz", mode.w, mode.h, mode.refresh_rate);
        state.log(msg);

        if (mode.refresh_rate != state.vid_refresh_rate) {
            state.log(" (ignored)\n");
            continue;
        }

        if (mode.w == state.config.vid_width &&
            mode.h == state.config.vid_height &&
            state.vid_mode == -1
        ) {
            state.vid_width = state.config.vid_width;
            state.vid_height = state.config.vid_height;
            state.vid_mode = m;

            state.log(" (ok, prefered by configuration)\n");
        } else {
            state.log(" (ok)\n");
        }

        state.vid_modes.insert(make_pair(m++, mode));
    }

    if (state.vid_modes.size() == 0) {
        state.log("No valid display mode found.\n");
        return false;
    }

    if (state.vid_mode == -1) {
        auto m = state.vid_modes.begin();

        state.vid_mode = m->first;
        state.vid_width = m->second.w;
        state.vid_height = m->second.h;
    }

    state.vid_quality = state.config.vid_quality;
    state.vid_vsync = state.config.vid_vsync;
    state.vid_fullscreen = state.config.vid_fullscreen;
    state.vid_aspect = 0;

    state.log("Initializing OpenGL context...\n");

    switch (state.vid_quality) {
        case QL_ULTRA:
            state.log("- multisampling enabled (8x)\n");
            state.vid_multisampling = 8;
            state.vid_fb_size = 4096;
            state.vid_font_resolution = 7;
            break;

        case QL_VERY_HIGH:
            state.log("- multisampling enabled (4x)\n");
            state.vid_multisampling = 4;
            state.vid_fb_size = 4096;
            state.vid_font_resolution = 6;
            break;

        case QL_HIGH:
            state.log("- multisampling enabled (4x)\n");
            state.vid_multisampling = 4;
            state.vid_fb_size = 2048;
            state.vid_font_resolution = 6;
            break;

        case QL_MEDIUM:
            state.log("- multisampling enabled (2x)\n");
            state.vid_multisampling = 2;
            state.vid_fb_size = 2048;
            state.vid_font_resolution = 5;
            break;

        case QL_LOW:
            state.log("- multisampling enabled (2x)\n");
            state.vid_multisampling = 2;
            state.vid_fb_size = 1024;
            state.vid_font_resolution = 5;
            break;

        default:
            state.log("- multisampling disabled\n");
            state.vid_multisampling = 0;
            state.vid_fb_size = 1024;
            state.vid_font_resolution = 4;
    }

    if (state.vid_fullscreen) {
        flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;
        state.log("- fullscreen mode\n");
    } else {
        flags = SDL_WINDOW_OPENGL;
        state.log("- window mode\n");
    }

    if (initDisplay(flags) == false) {
        state.log("Failed to initialize OpenGL context.\n");
        return false;
    }

    if (state.vid_fullscreen) {
        SDL_GetCurrentDisplayMode(state.vid_display, &current);

        for (auto i = state.vid_modes.begin(); i != state.vid_modes.end(); i++) {
            if (current.w == i->second.w &&
                current.h == i->second.h
            ) {
                state.vid_mode = i->first;
                state.vid_width = i->second.w;
                state.vid_height = i->second.h;
            }
        }
    }

    snprintf(msg, sizeof(msg), "- screen size %d x %d\n", state.vid_width, state.vid_height);
    state.log(msg);

    // initialize audio

    if (state.config.aud_sfx || state.config.aud_music) {
        snprintf(msg, sizeof(msg), "Initializing audio device at %d Hz... ", state.config.aud_mixfreq);
        state.log(msg);

        if (Mix_OpenAudio(state.config.aud_mixfreq, DEFAULT_AUD_FORMAT, 2, 1024) == -1) {
            state.log("failed\n");
            state.config.aud_sfx = -1;
            state.config.aud_music = -1;
        } else {
            state.log("ok\n");
            state.audio.init(
                state.dir_resources,
                state.config.aud_sfx,
                state.config.aud_music,
                state.config.aud_mixfreq
            );
        }
    }

    scene = make_unique<Scene>(&state);
    input = make_unique<Input>(&state);

    state.set(STATE_MENU);

    return true;
}

bool Engine::initDisplay(int mode) {
    window = NULL;
    context = NULL;

    do {
        int multisampling;

        if (context != NULL) {
            SDL_GL_DeleteContext(context);
        }

        if (window != NULL) {
            SDL_DestroyWindow(window);
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        if (state.vid_multisampling > 0) {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, state.vid_multisampling);
        } else {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
        }

        window = SDL_CreateWindow(
            "Lift Off: Beyond Glaxium",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            state.vid_width,
            state.vid_height,
            mode
        );

        context = SDL_GL_CreateContext(window);

        SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &multisampling);

        if (state.vid_multisampling == multisampling) {
            SDL_GL_MakeCurrent(window, context);

            if (state.vid_vsync) {
                SDL_GL_SetSwapInterval(1);
            } else {
                SDL_GL_SetSwapInterval(0);
            }
            break;
        }

        if (state.vid_multisampling > 0) {
            state.vid_multisampling = state.vid_multisampling >> 1;
        }
    } while (true);

    // SDL

    SDL_ShowWindow(window);
    SDL_ShowCursor(0);

    // GLEW

    GLint glewStatus = glewInit();

    if (GLEW_OK != glewStatus) {
        return false;
    }

    // OpenGL

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_TRUE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void Engine::halt() {
    state.set(STATE_QUIT);

    if (state.config.vid_fullscreen == 0) {
        SDL_ShowCursor(1);
    }

    if (state.joystick != NULL) {
        SDL_JoystickClose(state.joystick);
    }

    state.log("Saving configuration... ");

    if (Configuration::save(state.dir_configuration, &state.config)) {
        state.log("ok\n");
    } else {
        state.log("failed\n");
    }

    if (state.config.aud_sfx   != -1 ||
        state.config.aud_music != -1
    ) {
        state.log("Closing audio device\n");

        Mix_CloseAudio();
        SDL_CloseAudio();
    }

    state.log("Closing display\n");

    buffer.reset();
    scene.reset();
    input.reset();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Engine::main() {
    input->handle();

    if (state.vid_aspect == 0) {
        state.vid_aspect = static_cast<float>(state.vid_width) / static_cast<float>(state.vid_height);

        state.view.reset();
        state.view = View::createPerspective(65, state.vid_aspect, .1f, 10000.0f);

        buffer.reset();
        buffer = make_unique<Renderbuffer>(&state);
    }

    scene->update();
    scene->draw(buffer);

    SDL_GL_SwapWindow(window);

    if (state.get() == STATE_RESTART) {
        halt();

        if (init(-1, NULL) == false) {
            state.set(STATE_CLOSE);
        }
    }

    return (state.get() != STATE_CLOSE);
}
