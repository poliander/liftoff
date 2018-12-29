#include "engine.hpp"
#include "state.hpp"

Engine::Engine() {
	state = new State();
	scenery = NULL;
	srand(time(NULL));
}

Engine::~Engine() {
	if (scenery != NULL) delete scenery;
	delete state;
}

/*
 * load video & audio settings from file
 */
bool Engine::loadConfiguration() {
	bool result = true;
	char cfgdir[255], cwd[255];
	FILE *fp = NULL;

	getcwd(cwd, 255);

	// game resources located in ./dat ?
	if (chdir(state->engine_datadir) != 0) {

		// no, so use compiled-in game data path
		sprintf(cfgdir, "%s/games/liftoff", GAME_DATA);

		if (chdir(cfgdir) == 0) {
			sprintf(state->engine_datadir, "%s", cfgdir);
		} else {
			sprintf(state->engine_datadir, "%s", "");
		}
	}
	chdir(cwd);

	fp = fopen(state->engine_cfgfile, "rb");
	if (fp == NULL) {
		result = false;
	} else {
		if (fread(&state->config, sizeof(config_t), 1, fp) != 1) {
			result = false;
		}

		fclose(fp);
	}

	return result;
}

/*
 * store video & audio settings in file
 */
bool Engine::writeConfiguration() {
	char cfgdir[255], cwd[255];
	FILE *fp = NULL;
	bool result = true;

	getcwd(cwd, 255);
	sprintf(cfgdir, "%s/.liftoff", getenv("HOME"));
	if (chdir(cfgdir) != 0) {
#ifdef WINDOWS
		mkdir(cfgdir);
#else
		mkdir(cfgdir, 0755);
#endif
	} else chdir(cwd);

	fp = fopen(state->engine_cfgfile, "wb");

	if (fp == NULL) {
		result = false;
	} else {
		if (fwrite(&state->config, sizeof(config_t), 1, fp) != 1) {
			result = false;
		}

		fclose(fp);
	}

	return result;
}

/*
 * get video mode ID
 */
void Engine::getVideoModeID() {
	state->vid_cfg_mode = -1;

	for (int i=0; i<state->vid_sup_modes_count; i++) {
		if ( (state->config.vid_width == state->vid_sup_modes[i]->w) &&
		     (state->config.vid_height == state->vid_sup_modes[i]->h) )
			     state->vid_cfg_mode = i;
	}
}

/*
 * engine initialization
 */
bool Engine::init(int argc, char **argv) {
	const SDL_VideoInfo *vidinfo;
	SDL_Rect **vidmodes;
	Uint32 modeok = false;
	int i;
	char *msg = new char[255];
	int defmodes[3][2] = {
		{ DEFAULT_VID_WIDTH, DEFAULT_VID_HEIGHT },
		{ -1, -1 },
		{ 800, 600 }
	};

	// parse command line parameters
	if (argc >= 0) {
		int cfg_multisampling = 0;
		bool cfg_lowquality = false;
		bool cfg_debug = false;

		for (int i=0; i<argc; i++) {

			// show command line parameters
			if (strcmp(argv[i], "-h") == 0) {
				printf("\nLift Off: Beyond Glaxium (%s)\n\n", PACKAGE_VERSION);
				printf("  -h            Show command line parameters\n");
				printf("  -l            Low quality mode\n");
				printf("  -m[2|4|8|16]  Enable multisampling, use 2/4/8/16 samples\n");
#ifdef WINDOWS
				printf("  -s            Enable logging to stdout.txt\n");
#endif
				printf("\n");

				return false;
			}

			if (strcmp(argv[i], "-m2") == 0) {
				cfg_multisampling = 2;
				continue;
			}

			if (strcmp(argv[i], "-m4") == 0) {
				cfg_multisampling = 4;
				continue;
			}

			if (strcmp(argv[i], "-m8") == 0) {
				cfg_multisampling = 8;
				continue;
			}

			if (strcmp(argv[i], "-m16") == 0) {
				cfg_multisampling = 16;
				continue;
			}

			if (strcmp(argv[i], "-m") == 0) {
				cfg_multisampling = 16;
				continue;
			}

			if (strcmp(argv[i], "-l") == 0) {
				cfg_lowquality = true;
				continue;
			}

			if (strcmp(argv[i], "-d") == 0) {
				cfg_debug = true;
				continue;
			}

#ifdef WINDOWS
			if (strcmp(argv[i], "-s") == 0) {
							state->log_stdout = true;
							continue;
			}
#endif
		}

		state->vid_cfg_lowquality = cfg_lowquality;
		state->vid_cfg_multisampling = cfg_multisampling;
		state->engine_debug = cfg_debug;
	}

	scenery = new Scenery(state);

	state->log("\nLift Off: Beyond Glaxium (");
	state->log(PACKAGE_VERSION);
	state->log(")\n\n");

	// SDL initalization
	state->log("Initializing Simple DirectMedia Layer\n");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK)) {
		sprintf(msg, "%s\n", SDL_GetError());
		state->log(msg);

		delete [] msg;
		return false;
	}

	// SDL joystick/gamepad initialization
	state->log("Initializing joystick or gamepad... ");
	if (SDL_NumJoysticks() > 0) {
		state->joystick = SDL_JoystickOpen(0);

		if (state->joystick) {
			state->log("ok\n");
		} else {
			state->log("failed\n");
		}
	} else {
		state->log("failed\n");
	}

	// get location of configuration file
	if (argv != NULL) {
#ifdef WINDOWS
		int l = strlen(argv[0]);
		char cfgdir[255], cwd[255];

		getcwd(cwd, 255);
		sprintf(cfgdir, "%s\\AppData\\Local", getenv("homepath"));

		if (chdir(cfgdir) == 0) {
			// AppData directory exists
			mkdir("Lift Off");
			if (chdir("Lift Off") == 0) {
				sprintf(state->engine_cfgfile, "%s\\Lift Off\\%s", cfgdir, DEFAULT_CFG_FILE);
			}
		} else {
			// Store liftoff.cfg in same directory as binary
			while ((l>0) && (argv[0][l] != '\\')) l--;
			strncpy(cfgdir, argv[0], l);
			cfgdir[l] = 0;
			sprintf(state->engine_cfgfile, "%s\\%s", cfgdir, DEFAULT_CFG_FILE);
		}

		chdir(cwd);
#else
		sprintf(state->engine_cfgfile, "%s/.liftoff/%s", getenv("HOME"), DEFAULT_CFG_FILE);
#endif
	}

	// load configuration
	state->log("Loading configuration... ");
	if (!loadConfiguration()) {
		state->log("failed (using defaults)\n");
	} else {
		state->log("ok\n");
	}
	defmodes[0][0] = state->config.vid_width;
	defmodes[0][1] = state->config.vid_height;

	// check for OpenGL/screen capabilities
	state->log("Validating display configuration...\n");
	vidinfo = SDL_GetVideoInfo();
	sprintf(msg, "- %dx%d screen found\n", vidinfo->current_w, vidinfo->current_h);
	state->log(msg);
	for (i=0; i<2; i++) {
		if (defmodes[i][0] == -1) {
			defmodes[i][0] = vidinfo->current_w;
			defmodes[i][1] = vidinfo->current_h;
		}
	}
	state->vid_cfg_depth = vidinfo->vfmt->BitsPerPixel;
	vidmodes = SDL_ListModes(NULL, SDL_FULLSCREEN);
	state->vid_sup_fullscreen = true;
	if (vidmodes == (SDL_Rect **)0) {
		state->vid_sup_window = false;
	}
	vidmodes = SDL_ListModes(NULL, SDL_RESIZABLE);
	state->vid_sup_window = true;
	if (vidmodes == (SDL_Rect **)0) {
		state->vid_sup_fullscreen = false;
	}
	if ( (state->vid_sup_window == false) &&
	     (state->config.vid_fullscreen == 0) ) {
		state->config.vid_fullscreen = 1;
	}
	if ( (state->vid_sup_fullscreen == false) &&
	     (state->config.vid_fullscreen == 1) ) {
		state->config.vid_fullscreen = 0;
	}
	vidmodes = SDL_ListModes(NULL, SDL_RESIZABLE | SDL_FULLSCREEN);
	if (vidmodes == (SDL_Rect **)0) {
		state->log("No valid video mode found\n");
		return false;
	}
	for(i=0; vidmodes[i]; ++i);
	state->vid_sup_modes_count = i;
	state->vid_sup_modes = vidmodes;
	if (i>0) {
		defmodes[2][0] = state->vid_sup_modes[i-1]->w;
		defmodes[2][1] = state->vid_sup_modes[i-1]->h;
	} else {
		state->log("No valid video mode found\n");

		delete [] msg;
		return false;
	}
	for (i=0; i<3; i++) {
		if (state->config.vid_fullscreen) {
			modeok = SDL_VideoModeOK(defmodes[i][0], defmodes[i][1], state->vid_cfg_depth, SDL_FULLSCREEN);
			if (modeok) {
				state->config.vid_width = defmodes[i][0];
				state->config.vid_height = defmodes[i][1];
				break;
			}
		} else {
			modeok = SDL_VideoModeOK(defmodes[i][0], defmodes[i][1], state->vid_cfg_depth, SDL_RESIZABLE);
			if (modeok) {
				state->config.vid_width = defmodes[i][0];
				state->config.vid_height = defmodes[i][1];
				break;
			}
		}
	}
	if (!modeok) {
		state->log("No valid video mode found\n");

		delete [] msg;
		return false;
	} else {
		getVideoModeID();
	}

	sprintf(msg, "- %dx%d @ %d bpp ",
		state->config.vid_width,
		state->config.vid_height,
		state->vid_cfg_depth
	);
	state->log(msg);

	if (state->config.vid_fullscreen == 0) {
		state->log("window mode selected\n");
	} else {
		state->log("fullscreen mode selected\n");
	}

	state->log("- vsync ");
	if (state->config.vid_vsync == 0) {
		state->log("disabled\n");
	} else {
		state->log("enabled\n");
	}

	state->log("- multisampling ");
	if (state->vid_cfg_multisampling > 0) {
		sprintf(msg, "enabled (%d samples)\n",
			state->vid_cfg_multisampling);
		state->log(msg);
	} else {
		state->log("disabled\n");
	}

	// OpenGL initialization
	state->log("Initializing OpenGL display\n");
	if (!initDisplay()) {
		state->log(SDL_GetError());
		state->log("\n\n");

		delete [] msg;
		return false;
	}

	// audio initalization
	if (state->config.aud_sfx || state->config.aud_music) {
		sprintf(msg, "Initializing audio device (%d Hz)\n", state->config.aud_mixfreq);
		state->log(msg);

		if (Mix_OpenAudio(state->config.aud_mixfreq, DEFAULT_AUD_FORMAT, 2, 1024) == -1) {
			state->log("Couldn't initialize audio device, SFX and music disabled\n");
			state->config.aud_sfx = -1;
			state->config.aud_music = -1;
		} else {
			state->audio->init(
					state->engine_datadir,
					state->config.aud_sfx,
					state->config.aud_music,
					state->config.aud_mixfreq
			);
		}
	}

	// load game resources
	scenery->load();

	if (state->engine_restart || state->engine_debug) {
		state->set(STATE_MENU);
	} else {
		state->set(STATE_INTRO);
	}

	delete [] msg;
	return true;
}

/*
 * engine shutdown
 */
void Engine::shutdown() {
	if (state->config.vid_fullscreen == 0) {
		SDL_ShowCursor(1);
	}

	if (state->joystick != NULL) {
		SDL_JoystickClose(state->joystick);
	}

	if ( (state->config.aud_sfx != -1) ||
	     (state->config.aud_music != -1) ) {
		state->log("Closing audio device\n");
		Mix_CloseAudio();
		SDL_CloseAudio();
	}

	state->log("Closing OpenGL display\n");
	if (state->vid_cfg_multisampling > 0) {
		glDisable(GL_MULTISAMPLE);
	}

	SDL_Quit();

	state->log("Saving configuration... ");
	if (writeConfiguration()) {
		state->log("ok\n");
	} else {
		state->log("failed\n");
	}

	if (!state->engine_restart) state->log("\n");
}

/*
 * OpenGL screen initalization
 */
bool Engine::initDisplay() {
	int result;
	bool throttle_multisampling = false;
	char msg[255];

	switch (state->config.vid_aspect) {
		case 1:
			state->vid_cfg_aspect = 4.0f/3.0f;
			break;
		case 2:
			state->vid_cfg_aspect = 16.0f/9.0f;
			break;
		case 3:
			state->vid_cfg_aspect = 1.6f;
			break;
		default:
			state->vid_cfg_aspect = (float)state->config.vid_width/(float)state->config.vid_height;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (state->vid_cfg_multisampling > 0) {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, state->vid_cfg_multisampling);
	}

	if (state->config.vid_vsync) {
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	} else {
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);
	}

	if (state->config.vid_fullscreen) {
		screen = SDL_SetVideoMode(state->config.vid_width, state->config.vid_height, state->vid_cfg_depth, SDL_OPENGL | SDL_FULLSCREEN);
	} else {
		screen = SDL_SetVideoMode(state->config.vid_width, state->config.vid_height, state->vid_cfg_depth, SDL_OPENGL);
	}

	// catch multisampling failure
	while ((screen == NULL) && (state->vid_cfg_multisampling > 0)) {
		SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &result);

		if (result != state->vid_cfg_multisampling) {
			throttle_multisampling = true;

			switch (state->vid_cfg_multisampling) {
				case 16:
					state->vid_cfg_multisampling = 8;
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, state->vid_cfg_multisampling);
					break;
				case 8:
					state->vid_cfg_multisampling = 4;
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, state->vid_cfg_multisampling);
					break;
				case 4:
					state->vid_cfg_multisampling = 2;
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, state->vid_cfg_multisampling);
					break;
				case 2:
					state->vid_cfg_multisampling = 0;
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
					break;
			}
		}

		if (state->config.vid_fullscreen) {
			screen = SDL_SetVideoMode(state->config.vid_width, state->config.vid_height, state->vid_cfg_depth, SDL_OPENGL | SDL_FULLSCREEN);
		} else {
			screen = SDL_SetVideoMode(state->config.vid_width, state->config.vid_height, state->vid_cfg_depth, SDL_OPENGL);
		}
	}

	// not possible to initialize OpenGL screen
	if (screen == NULL) return false;

	// had to throttle multisampling...
	if (throttle_multisampling) {
		if (state->vid_cfg_multisampling > 0) {
			sprintf(msg, "Multisampling throttled to %d samples\n", state->vid_cfg_multisampling);
		} else {
			sprintf(msg, "Could not enable multisampling!\n");
		}

		state->log(msg);
	}

	SDL_ShowCursor(0);
	SDL_WM_SetCaption("Lift Off: Beyond Glaxium", NULL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, state->config.vid_width, state->config.vid_height);
	glFrustum(-400.0f*state->vid_cfg_aspect, 400.0f*state->vid_cfg_aspect, -300.0f, 300.0f, .1f, 10000.0f);
	glLoadIdentity();
	gluPerspective(65, state->vid_cfg_aspect, .1f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	if (state->vid_cfg_multisampling > 0) glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(.0f, .0f, .0f, .0f);
	glClearDepth(1.0f);
	glLoadIdentity();

	state->vid_cfg_width = state->config.vid_width;
	state->vid_cfg_height = state->config.vid_height;
	state->vid_cfg_aspect_mode = state->config.vid_aspect;
	state->vid_cfg_fullscreen = state->config.vid_fullscreen;
	state->vid_cfg_vsync = state->config.vid_vsync;

	return true;
}

/*
 * keyboard handler
 */
bool Engine::keyHandler() {
	Uint8 *keys = SDL_GetKeyState(NULL);

	static GLuint timer = state->timer - 51;
	static GLuint nextrelease = state->timer - 100;

	bool moved = false;

	if ((state->get() < 11) && ((state->timer - timer) < 50)) {
		return moved;
	}

	// STRG+C: Quit immediately
	if (keys[SDLK_LCTRL] && keys[SDLK_c]) {
		state->set(STATE_CLOSE);
		return moved;
	}

	// F12: toggle FPS display
	if (keys[SDLK_F12]) {
		if (state->timer > nextrelease) {
			state->fps_visible =! state->fps_visible;
			nextrelease = state->timer + 100;
			if (state->fps_visible) {
				state->fps = 0;
				state->fps_frame = 0;
				state->fps_count = 100;
				state->fps_ready = false;
			}
		} else nextrelease = state->timer + 100;
	}

	timer = state->timer;

	switch(state->get()) {

		case STATE_GAME_LOOP:
			if (keys[SDLK_ESCAPE])
				state->set(STATE_GAME_QUIT);

			if (state->objects[state->player].life > 0) {

				// Keyboard LEFT, RIGHT
				if (keys[SDLK_LEFT] || keys[SDLK_a]) {
					scenery->player->accelerateX(state->player, 1.0f);
					moved = true;
				} else if (keys[SDLK_RIGHT] || keys[SDLK_d]) {
					scenery->player->accelerateX(state->player, -1.0f);
					moved = true;
				}

				// Keyboard UP, DOWN
				if (keys[SDLK_UP] || keys[SDLK_w]) {
					scenery->player->accelerateY(state->player, -1.0f);
					moved = true;
				} else if (keys[SDLK_DOWN] || keys[SDLK_s]) {
					scenery->player->accelerateY(state->player, 1.0f);
					moved = true;
				}

				// Keyboard CTRL
				if (keys[SDLK_LCTRL] || keys[SDLK_RCTRL]) {
					scenery->player->shoot();
				}
			}
			break;

		case STATE_MENU:
			if (keys[SDLK_ESCAPE]) {
				state->audio->playSample(1, 128, 0);

				switch(state->menu) {
					case 4: // audio
						state->menu_pos = 3;
						state->menu_selected = true;
						state->config.aud_sfx = state->audio->volume_sfx;
						state->config.aud_music = state->audio->volume_music;
						state->config.aud_mixfreq = state->audio->mixer_frequency;
						break;

					case 3: // video
						state->menu_pos = 4;
						state->menu_selected = true;
						for (int i=0; i<state->vid_sup_modes_count; i++) {
							if ( (state->vid_cfg_width  == state->vid_sup_modes[i]->w) &&
							     (state->vid_cfg_height == state->vid_sup_modes[i]->h) ) {
							     state->vid_cfg_mode = i;
							}
						}
						state->config.vid_aspect = state->vid_cfg_aspect_mode;
						state->config.vid_fullscreen = state->vid_cfg_fullscreen;
						state->config.vid_vsync = state->vid_cfg_vsync;
						break;

					default: // main menu
						state->menu_pos = 2;
						state->menu_selected = true;
				}

				return moved;
			}

			if (keys[SDLK_RETURN]) {
				state->menu_selected = true;
				if ( (state->menu == 1 && state->menu_pos == 2) ||
					 (state->menu == 2 && state->menu_pos == 2) ) {
					state->audio->playSample(1, 128, 0); // cancel
				} else {
					state->audio->playSample(0, 128, 0); // ok
				}
				return moved;
			}

			if (keys[SDLK_UP] || keys[SDLK_w]) {
				state->menu_pos--;
				return moved;
			}

			if (keys[SDLK_DOWN] || keys[SDLK_s]) {
				state->menu_pos++;
				return moved;
			}
	}

	return moved;
}

/*
 * joystick handler
 */
void Engine::joyHandler() {
	float v;

	SDL_JoystickUpdate();

	if (state->objects[state->player].life <= 0) return;

	v = float(SDL_JoystickGetAxis(state->joystick, 0) * .00003f);
	if (fabs(v) > .01f) {
		scenery->player->accelerateX(state->player, float(scenery->player->acceleration) * -.0075f * v);
	}

	v = float(SDL_JoystickGetAxis(state->joystick, 1) * .00003f);
	if (fabs(v) > .01f) {
		scenery->player->accelerateY(state->player, float(scenery->player->acceleration) * .0075f * v);
	}

	if (SDL_JoystickGetButton(state->joystick, 0) != 0) {
		scenery->player->shoot();
	}
}

/*
 * mouse handler
 */
void Engine::mouseHandler() {
	int x, y;

	if (state->config.vid_fullscreen == 0) {
		if (!(SDL_GetAppState()&SDL_APPMOUSEFOCUS)) return;
	}

	state->mouse_button = SDL_GetMouseState(&x, &y);

	if (state->mouse_button & SDL_BUTTON(1)) {
		if (!state->mouse_pressed) {
			state->mouse_button = SDL_GetMouseState(&x, &y);
			if (state->mouse_released) {
				state->mouse_pressed = true;
			}
		} else {
			state->mouse_button = 0;
		}
		state->mouse_released = false;
	} else {
		state->mouse_pressed = false;
		state->mouse_released = true;
		SDL_GetMouseState(&x, &y);
	}

	if (state->get() == STATE_MENU) {
		state->mouse_moved = true;
		state->mouse_x = (-3.1f * state->vid_cfg_aspect) + (1 / float(state->config.vid_width)) * x * (6.35f * state->vid_cfg_aspect);
		state->mouse_y = 3.1f + (-1 / float(state->config.vid_height)) * y * 6.35f;
	}
}

/*
 * main game loop
 */
bool Engine::main() {
	SDL_Event event;

	// timer adjustment
	static GLuint otimer = glutGet((GLenum)(GLUT_ELAPSED_TIME));
	GLuint ntimer = glutGet((GLenum)(GLUT_ELAPSED_TIME));
	state->timer_adjustment = float(ntimer-otimer) * .05f;
	state->timer = ntimer;
	otimer = ntimer;

	// complete restart of game engine
	if (state->engine_restart) {
		state->log("Restarting game engine.\n");

		state->config.vid_width = state->vid_sup_modes[state->vid_cfg_mode]->w;
		state->config.vid_height = state->vid_sup_modes[state->vid_cfg_mode]->h;
		state->set(STATE_QUIT);

		shutdown();

		delete scenery;
		scenery = new Scenery(state);

		bool ok = init(-1, NULL);
		otimer = glutGet((GLenum)(GLUT_ELAPSED_TIME));
		state->timer = otimer;

		return ok;
	}

	state->mouse_moved = false;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				state->set(STATE_QUIT);
				break;

			case SDL_KEYDOWN:
				keyHandler();
				break;

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				mouseHandler();
				break;
		}
	}

	if (state->get() == STATE_GAME_LOOP) {
		if (!keyHandler() && (state->joystick != NULL)) joyHandler();
	}

	scenery->move();
	scenery->draw();

	SDL_GL_SwapBuffers();

	return (state->get() != STATE_CLOSE);
}
