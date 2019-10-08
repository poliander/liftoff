#include "engine.hpp"

Engine::Engine()
{
    srand((int)time(NULL));
}

Engine::~Engine()
{
}

/*
 * load video & audio settings from file
 */
bool Engine::loadConfiguration()
{
    bool result = false;
    char cwd[255];

    getcwd(cwd, 255);

    if (chdir(state.dir_configuration) == 0) {
        FILE *fp = fopen(CFG_FILENAME, "rb");

        if (fp) {
            if (fread(&state.config, sizeof(Configuration), 1, fp) == 1) {
                result = true;
            }

            fclose(fp);
        }
    }

    chdir(cwd);

    return result;
}

/*
 * store video & audio settings in file
 */
bool Engine::writeConfiguration()
{
    bool result = false;
    char cwd[255];

    getcwd(cwd, 255);

    if (chdir(state.dir_configuration) != 0) {
#ifdef _WIN32
        mkdir(state.dir_configuration);
#else
        mkdir(state.dir_configuration, 0755);
#endif
    }

    if (chdir(state.dir_configuration) == 0) {
        FILE *fp = fopen(CFG_FILENAME, "wb");

        if (fp) {
            if (fwrite(&state.config, sizeof(Configuration), 1, fp) == 1) {
                result = true;
            }

            fclose(fp);
        }
    }

    chdir(cwd);

    return result;
}

/*
 * engine initialization
 */
bool Engine::init(int argc, char **argv)
{
    char *msg = new char[255];
    SDL_DisplayMode current;

    if (argc >= 0) {
        for (int i=0; i<argc; i++) {
            if (strcmp(argv[i], "-noms") == 0) {
                state.vid_multisampling = 0;
                continue;
            }

            if (strcmp(argv[i], "-d") == 0) {
                state.log_file = true;
                continue;
            }
        }
    }

    // load configuration

    state.log("Loading configuration... ");

    if (loadConfiguration()) {
        state.cfg_loaded = true;
        state.log("ok\n");
    } else {
        state.log("failed (using defaults)\n");
    }

    // initialize SDL

    state.log("Initializing SDL... ");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK)) {
        sprintf(msg, "failed (%s)\n", SDL_GetError());
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

    // initialize screen

    state.log("Initializing display... ");

    for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i) {
        if (0 == SDL_GetCurrentDisplayMode(i, &current)) {
            sprintf(msg, "\n- screen #%d is %d x %d @ %d bpp", i, current.w, current.h, SDL_BITSPERPIXEL(current.format));

            if (state.vid_display == -1) {
                state.vid_display = i;
                state.vid_format = current.format;
                state.vid_refresh_rate = current.refresh_rate;
                state.vid_width = current.w;
                state.vid_height = current.h;

                sprintf(msg, "%s (selected)", msg);
            }

            state.log(msg);
        }
    }

    if (state.vid_display == -1) {
        state.log("failed, no screen found\n");
        return false;
    } else {
        state.log("\n");
    }

    state.vid_cap_modes_num = 0;

    SDL_DisplayMode mode;

    for (int i = 0; i < SDL_GetNumDisplayModes(state.vid_display); i++) {
        SDL_GetDisplayMode(state.vid_display, i, &mode);

        if (mode.format != state.vid_format) {
            continue;
        }

        if (mode.refresh_rate != state.vid_refresh_rate) {
            continue;
        }

        state.vid_cap_modes[state.vid_cap_modes_num++] = mode;
    }

    if (state.cfg_loaded) {
        sprintf(msg, "- configuration is %d x %d", state.config.vid_width, state.config.vid_height);

        if (state.config.vid_fullscreen) {
            sprintf(msg, "%s (fullscreen mode)\n", msg);
        } else {
            sprintf(msg, "%s (window mode)\n", msg);
        }

        state.log(msg);

        for (int i = 0; i < state.vid_cap_modes_num; i++) {
            if (state.vid_cap_modes[i].w == state.config.vid_width &&
                state.vid_cap_modes[i].h == state.config.vid_height
            ) {
                state.vid_width = state.config.vid_width;
                state.vid_height = state.config.vid_height;
                state.vid_fullscreen = state.config.vid_fullscreen;
                state.vid_aspect = state.config.vid_aspect;
                state.vid_vsync = state.config.vid_vsync;
                break;
            }
        }
    }

    Uint32 win_flags = SDL_WINDOW_OPENGL;

    if (state.vid_fullscreen) {
        win_flags = SDL_WINDOW_OPENGL & SDL_WINDOW_FULLSCREEN;
    }

    state.vid_mode = -1;

    for (int i = 0; i < state.vid_cap_modes_num; i++) {
        if (state.vid_cap_modes[i].w == state.vid_width &&
            state.vid_cap_modes[i].h == state.vid_height
        ) {
            state.vid_mode = i;
            initDisplay();
            break;
        }
    }

    // initialize audio

    if (state.config.aud_sfx || state.config.aud_music) {
        sprintf(msg, "Initializing audio device at %d Hz... ", state.config.aud_mixfreq);
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

    scene = new Scene(state);
    scene->load();

    state.set(STATE_MENU);

    return true;
}

/*
 * OpenGL screen initalization
 */
bool Engine::initDisplay()
{
    int cfg_multisampling, sdl_mode = SDL_WINDOW_OPENGL;
    char msg[255];

    switch (state.config.vid_aspect) {
        case 1:
            state.vid_aspect = 4.0f / 3.0f;
            break;

        case 2:
            state.vid_aspect = 16.0f / 9.0f;
            break;

        case 3:
            state.vid_aspect = 16.0f / 10.0f;
            break;

        default:
            state.vid_aspect = (float)state.vid_width / (float)state.vid_height;
    }

    if (state.config.vid_fullscreen) {
        sdl_mode |= SDL_WINDOW_FULLSCREEN;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (state.vid_multisampling > 0) {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, state.vid_multisampling);
    }

    window = SDL_CreateWindow(
        "Lift Off: Beyond Glaxium",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        state.config.vid_width,
        state.config.vid_height,
        sdl_mode
    );

    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &cfg_multisampling);

    while (window == NULL && state.vid_multisampling != cfg_multisampling) {
        switch (state.vid_multisampling) {
            case 8:
                state.vid_multisampling = 4;
                SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
                SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, state.vid_multisampling);
                break;

            case 4:
                state.vid_multisampling = 2;
                SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
                SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, state.vid_multisampling);
                break;

            case 2:
                state.vid_multisampling = 0;
                SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
                break;

            case 0:
               return false;
        }

        window = SDL_CreateWindow(
            "Lift Off: Beyond Glaxium",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            state.config.vid_width,
            state.config.vid_height,
            sdl_mode
        );

        SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &cfg_multisampling);
    }

    context = SDL_GL_CreateContext(window);

    if (state.config.vid_vsync) {
        SDL_GL_SetSwapInterval(1);
    } else {
        SDL_GL_SetSwapInterval(0);
    }

    GLint glewStatus = glewInit();

    if (GLEW_OK != glewStatus) {
        sprintf(msg, "\nGLEW ERROR: %s\n", glewGetErrorString(glewStatus));
        state.log(msg);

        return false;
    }

    SDL_ShowCursor(0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-400.0f * state.vid_aspect, 400.0f * state.vid_aspect, -300.0f, 300.0f, .1f, 10000.0f);

    glLoadIdentity();
    gluPerspective(65, state.vid_aspect, .1f, 10000.0f);
    state.view.perspective(65, state.vid_aspect, .1f, 10000.0f);
    glMatrixMode(GL_MODELVIEW);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    if (state.vid_multisampling > 0) glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(.0f, .0f, .0f, .0f);
    glClearDepth(1.0f);
    glLoadIdentity();

    state.vid_width = state.config.vid_width;
    state.vid_height = state.config.vid_height;
    state.vid_aspect_mode = state.config.vid_aspect;
    state.vid_fullscreen = state.config.vid_fullscreen;
    state.vid_vsync = state.config.vid_vsync;

    return true;
}

/*
 * keyboard handler
 */
bool Engine::handleKeyboard()
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    static GLuint timer = state.timer - 51;
    static GLuint nextrelease = state.timer - 100;

    bool moved = false;

    if ((state.get() < 11) && ((state.timer - timer) < 50)) {
        return moved;
    }

    // STRG+C: Quit immediately
    if (keys[SDL_SCANCODE_LCTRL] && keys[SDL_SCANCODE_C]) {
        state.set(STATE_CLOSE);
        return moved;
    }

    // F12: toggle FPS display
    if (keys[SDL_SCANCODE_F12]) {
        if (state.timer > nextrelease) {
            state.fps_visible =! state.fps_visible;
            nextrelease = state.timer + 100;
            if (state.fps_visible) {
                state.fps = 0;
                state.fps_frame = 0;
                state.fps_count = 100;
                state.fps_ready = false;
            }
        } else nextrelease = state.timer + 100;
    }

    timer = state.timer;

    switch (state.get()) {

        case STATE_GAME_LOOP:
            if (keys[SDL_SCANCODE_ESCAPE])
                state.set(STATE_GAME_QUIT);

            if (state.player->isAlive()) {

                // Keyboard LEFT, RIGHT
                if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
                    state.player->setAccelerationX(1.0f);
                    moved = true;
                } else if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
                    state.player->setAccelerationX(-1.0f);
                    moved = true;
                }

                // Keyboard UP, DOWN
                if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
                    state.player->setAccelerationY(-1.0f);
                    moved = true;
                } else if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
                    state.player->setAccelerationY(1.0f);
                    moved = true;
                }

                // Keyboard CTRL
                if (keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL]) {
                    state.player->shoot(state);
                }
            }
            break;

        case STATE_MENU:
            if (keys[SDL_SCANCODE_ESCAPE]) {
                state.audio.playSample(1, 128, 0);

                switch (state.menu) {
                    case 4: // audio
                        state.menu_pos = 3;
                        state.menu_selected = true;
                        state.config.aud_sfx = state.audio.volume_sfx;
                        state.config.aud_music = state.audio.volume_music;
                        state.config.aud_mixfreq = state.audio.mixer_frequency;
                        break;

                    case 3: // video
                        state.menu_pos = 4;
                        state.menu_selected = true;

                        for (int i=0; i<state.vid_cap_modes_num; i++) {
                            if ( (state.vid_width  == state.vid_cap_modes[i].w) &&
                                 (state.vid_height == state.vid_cap_modes[i].h) ) {
                                 state.vid_mode = i;
                            }
                        }

                        state.config.vid_aspect = state.vid_aspect_mode;
                        state.config.vid_fullscreen = state.vid_fullscreen;
                        state.config.vid_vsync = state.vid_vsync;
                        break;

                    default: // main menu
                        state.menu_pos = 2;
                        state.menu_selected = true;
                }

                return moved;
            }

            if (keys[SDL_SCANCODE_RETURN]) {
                state.menu_selected = true;

                if ( (state.menu == 1 && state.menu_pos == 2) ||
                     (state.menu == 2 && state.menu_pos == 2) ) {
                    state.audio.playSample(1, 128, 0); // cancel
                } else {
                    state.audio.playSample(0, 128, 0); // ok
                }

                return moved;
            }

            if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
                state.menu_pos--;
                return moved;
            }

            if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
                state.menu_pos++;
                return moved;
            }
    }

    return moved;
}

/*
 * joystick handler
 */
void Engine::handleJoystick()
{
    float v;

    if (state.player->isAlive() == false) {
        return;
    }

    SDL_JoystickUpdate();

    v = float(SDL_JoystickGetAxis(state.joystick, 0) * .00003f);

    if (fabs(v) > .01f) {
        state.player->setAccelerationX(float(state.player->getAcceleration()) * -.0075f * v);
    }

    v = float(SDL_JoystickGetAxis(state.joystick, 1) * .00003f);

    if (fabs(v) > .01f) {
        state.player->setAccelerationY(float(state.player->getAcceleration()) * .0075f * v);
    }

    if (SDL_JoystickGetButton(state.joystick, 0) != 0) {
        state.player->shoot(state);
    }
}

/*
 * mouse handler
 */
void Engine::handleMouse()
{
    int x, y;

    if (state.config.vid_fullscreen == 0 && state.mouse_focus == false) {
        return;
    }

    state.mouse_button = SDL_GetMouseState(&x, &y);

    if (state.mouse_button & SDL_BUTTON(1)) {
        if (!state.mouse_pressed) {
            state.mouse_button = SDL_GetMouseState(&x, &y);
            if (state.mouse_released) {
                state.mouse_pressed = true;
            }
        } else {
            state.mouse_button = 0;
        }
        state.mouse_released = false;
    } else {
        state.mouse_pressed = false;
        state.mouse_released = true;
        SDL_GetMouseState(&x, &y);
    }

    if (state.get() == STATE_MENU) {
        state.mouse_moved = true;
        state.mouse_x = (-3.1f * state.vid_aspect) + (1 / float(state.config.vid_width)) * x * (6.35f * state.vid_aspect);
        state.mouse_y = 3.1f + (-1 / float(state.config.vid_height)) * y * 6.35f;
    }
}

/*
 * engine shutdown
 */
void Engine::halt()
{
    if (state.config.vid_fullscreen == 0) {
        SDL_ShowCursor(1);
    }

    if (state.joystick != NULL) {
        SDL_JoystickClose(state.joystick);
    }

    state.log("Saving configuration... ");

    if (writeConfiguration()) {
        state.log("ok\n");
    } else {
        state.log("failed\n");
    }

    if (
        state.config.aud_sfx   != -1 ||
        state.config.aud_music != -1
    ) {
        state.log("Closing audio device\n");

        Mix_CloseAudio();
        SDL_CloseAudio();
    }

    state.log("Closing display\n");

    if (state.vid_multisampling > 0) {
        glDisable(GL_MULTISAMPLE);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    delete scene;
}

/*
 * main game loop
 */
bool Engine::main()
{
    SDL_Event event;

    // timer adjustment
    static Uint32 otimer = SDL_GetTicks();
    GLuint ntimer = SDL_GetTicks();
    state.timer_adjustment = float(ntimer-otimer) * .05f;
    state.timer = ntimer;
    otimer = ntimer;

    // complete restart of game engine
    if (state.engine_restart) {
        state.log("Restarting game engine.\n");

        state.config.vid_width = state.vid_cap_modes[state.vid_mode].w;
        state.config.vid_height = state.vid_cap_modes[state.vid_mode].h;
        state.set(STATE_QUIT);

        halt();
        bool ok = init(-1, NULL);
        otimer = SDL_GetTicks();
        state.timer = otimer;

        return ok;
    }

    state.mouse_moved = false;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                state.set(STATE_QUIT);
                break;

            case SDL_KEYDOWN:
                handleKeyboard();
                break;

            case SDL_WINDOWEVENT_ENTER:
                state.mouse_focus = true;
                break;

            case SDL_WINDOWEVENT_LEAVE:
                state.mouse_focus = false; 
                break;

            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
                handleMouse();
                break;
        }
    }

    if (state.get() == STATE_GAME_LOOP && false == handleKeyboard()) {
        handleJoystick();
    }

    scene->move();
    scene->draw();

    SDL_GL_SwapWindow(window);

    return (state.get() != STATE_CLOSE);
}
