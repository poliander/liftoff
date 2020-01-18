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

#include "input.hpp"

Input::Input(State* s) : state(s) {
}

bool Input::handleKeyboard() {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    static GLuint timer = SDL_GetTicks() - 51;
    static GLuint nextrelease = timer - 100;

    bool moved = false;

    if ((state->get() <= STATE_GAME_START) && ((SDL_GetTicks() - timer) < 50)) {
        return moved;
    }

    timer = SDL_GetTicks();

    // STRG+C: Quit immediately
    if (keys[SDL_SCANCODE_LCTRL] && keys[SDL_SCANCODE_C]) {
        state->set(STATE_CLOSE);
        return moved;
    }

    // F12: toggle FPS display
    if (keys[SDL_SCANCODE_F12]) {
        if (SDL_GetTicks() > nextrelease) {
            state->fps_visible = !state->fps_visible;

            if (state->fps_visible) {
                state->fps_counter = 0;
                state->fps_timer = 0;
                state->fps_timer_l = 0;
            }
        }

        nextrelease = SDL_GetTicks() + 100;
    }

    switch (state->get()) {
        case STATE_GAME_LOOP:
            if (keys[SDL_SCANCODE_ESCAPE]) {
                state->set(STATE_GAME_QUIT);
            }

            if (state->player->isAlive()) {
                // Keyboard LEFT, RIGHT
                if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
                    state->player->setAccelerationX(1.0f);
                    moved = true;
                } else if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
                    state->player->setAccelerationX(-1.0f);
                    moved = true;
                }

                // Keyboard UP, DOWN
                if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
                    state->player->setAccelerationY(-1.0f);
                    moved = true;
                } else if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
                    state->player->setAccelerationY(1.0f);
                    moved = true;
                }

                // Keyboard CTRL
                if (keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL]) {
                    state->player->shoot();
                }
            }
            break;

        case STATE_MENU:
            if (keys[SDL_SCANCODE_ESCAPE]) {
                state->audio.playSample(1, 128, 0);

                switch (state->menu) {
                    case 4: // audio
                        state->menu_pos = 3;
                        state->menu_selected = true;
                        state->config.aud_sfx = state->audio.volume_sfx;
                        state->config.aud_music = state->audio.volume_music;
                        state->config.aud_mixfreq = state->audio.mixer_frequency;
                        break;

                    case 3: // video
                        state->menu_pos = 4;
                        state->menu_selected = true;

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

                    default: // main menu
                        state->menu_pos = 2;
                        state->menu_selected = true;
                }

                return moved;
            }

            if (keys[SDL_SCANCODE_RETURN]) {
                state->menu_selected = true;

                if (state->menu_pos == 2 && (state->menu == 1 || state->menu == 2)) {
                    state->audio.playSample(1, 128, 0); // cancel
                } else {
                    state->audio.playSample(0, 128, 0); // ok
                }

                return moved;
            }

            if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
                state->menu_pos--;
                return moved;
            }

            if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
                state->menu_pos++;
                return moved;
            }
    }

    return moved;
}

void Input::handleJoystick() {
    float v;

    if (state->player->isAlive() == false) {
        return;
    }

    SDL_JoystickUpdate();

    v = static_cast<float>(SDL_JoystickGetAxis(state->joystick, 0) * .00003f);

    if (fabs(v) > .01f) {
        state->player->setAccelerationX(static_cast<float>(state->player->getAcceleration()) * -.0075f * v);
    }

    v = static_cast<float>(SDL_JoystickGetAxis(state->joystick, 1) * .00003f);

    if (fabs(v) > .01f) {
        state->player->setAccelerationY(static_cast<float>(state->player->getAcceleration()) * .0075f * v);
    }

    if (SDL_JoystickGetButton(state->joystick, 0) != 0) {
        state->player->shoot();
    }
}

void Input::handle() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                state->set(STATE_QUIT);
                break;

            case SDL_KEYDOWN:
                handleKeyboard();
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    if (state->vid_width != event.window.data1 ||
                        state->vid_height != event.window.data2
                    ) {
                        state->vid_width = event.window.data1;
                        state->vid_height = event.window.data2;
                        state->vid_aspect = 0;
                    }
                }
                break;
        }
    }

    if (state->get() == STATE_GAME_LOOP && false == handleKeyboard()) {
        handleJoystick();
    }
}
