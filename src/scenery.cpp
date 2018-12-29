#include "scenery.hpp"

float isqrt(float f) {
	float fhalf = 0.5f * f;

	asm (
		"mov %1, %%eax;"
		"sar %%eax;"
		"mov $0x5F3759DF, %%ebx;"
		"sub %%eax, %%ebx;"
		"mov %%ebx, %0"
		:"=g"(f)
		:"g"(f)
		: "%eax", "%ebx"
	);

	f *= 1.5f - fhalf*f*f;

	return f;
}

Scenery::Scenery(State* sptr) {
	float x, y;

	state = sptr;

	object = new Object(state);
	player = new Player(state);
	asteroid = new Object(state);
	debris = new Object(state);
	explosion = new Explosion(state);
	cargo = new Cargo(state);
	powerup = new Powerup(state);

	if (state->vid_cfg_lowquality) {
		state->engine_stars /= 2;
		state->engine_stars_warp /= 2;
	}

	srand((int)time(NULL));

	// generate far stars
	for (int i=0; i<(state->engine_stars - state->engine_stars_warp); i++) {
		x = 0;
		y = 0;

		while ((fabs(x) < 200.0f) && (fabs(y) < 200.0f)) {
			x = (float)(rand()%800 - 400);
			y = (float)(rand()%800 - 400);
		}

		stars[i][0] = x;
		stars[i][1] = y;
		stars[i][2] = (float)(rand()%5000);
		stars[i][3] = (float)((rand()%100)*.01f);
	}

	// generate warp stars
	for (int i=(state->engine_stars - state->engine_stars_warp); i<state->engine_stars; i++) {
			x = 0;
			y = 0;

			while ((fabs(x) < 25.0f) && (fabs(y) < 25.0f)) {
				x = (float)(rand()%400 - 200);
				y = (float)(rand()%400 - 200);
			}

			stars[i][0] = x;
			stars[i][1] = y;
			stars[i][2] = (float)(rand()%5000);
			stars[i][3] = (float)((rand()%100)*.01f);
	}
}

Scenery::~Scenery() {
	delete cargo;
	delete explosion;
	delete debris;
	delete asteroid;
	delete player;
	delete powerup;

	if (state->audio->volume_music) {
		Mix_HaltMusic();
		Mix_FreeMusic(state->audio->music[0]);
	}

	if (state->audio->volume_sfx) {
		Mix_FreeChunk(state->audio->sample[10]);
		Mix_FreeChunk(state->audio->sample[9]);
		Mix_FreeChunk(state->audio->sample[8]);
		Mix_FreeChunk(state->audio->sample[7]);
		Mix_FreeChunk(state->audio->sample[6]);
		Mix_FreeChunk(state->audio->sample[5]);
		Mix_FreeChunk(state->audio->sample[4]);
		Mix_FreeChunk(state->audio->sample[3]);
		Mix_FreeChunk(state->audio->sample[2]);
		Mix_FreeChunk(state->audio->sample[1]);
		Mix_FreeChunk(state->audio->sample[0]);
	}
}

/*
 * load texture from file
 */
GLuint Scenery::loadTexture(const char *filename, bool mipmap) {
	char fname[255], lmsg[255];
	GLuint texture = 0;
	SDL_Surface *image;
	SDL_Surface *target;
	SDL_RWops *rwop;

	sprintf(lmsg, "Loading 'gfx/%s'... ", filename);
	state->log(lmsg);

	sprintf(fname, "%s/gfx/%s", state->engine_datadir, filename);
	rwop = SDL_RWFromFile(fname, "rb");
	image = IMG_LoadTGA_RW(rwop);
	if (!image) {
		state->log("failed\n");
		return false;
	}

	target = SDL_CreateRGBSurface(SDL_SWSURFACE, image->w, image->h, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			0x000000FF,
			0x0000FF00,
			0x00FF0000,
			0xFF000000
#else
			0xFF000000,
			0x00FF0000,
			0x0000FF00,
			0x000000FF
#endif
	);

	SDL_SetAlpha(image, 0, 0);
	SDL_BlitSurface(image, 0, target, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (mipmap && !state->vid_cfg_lowquality) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, target->pixels);

	SDL_FreeSurface(image);
	SDL_FreeSurface(target);

	state->log("ok\n");

	return texture;
}

/*
 * load game data
 */
void Scenery::load() {
//	state->texture[T_LOGO] 			= loadTexture("logo.tga", true);
	state->texture[T_TITLE] 		= loadTexture("title.tga", true);
	state->texture[T_FONT] 			= loadTexture("font.tga", true);
	state->texture[T_CURSOR]		= loadTexture("cursor.tga", true);
	state->texture[T_MENU_1]		= loadTexture("menu_1.tga", false);
	state->texture[T_MENU_2]		= loadTexture("menu_2.tga", false);
	state->texture[T_HUD_1] 		= loadTexture("hud_1.tga", true);
	state->texture[T_HUD_2] 		= loadTexture("hud_2.tga", true);
	state->texture[T_HUD_3] 		= loadTexture("hud_3.tga", true);
	state->texture[T_STAR] 			= loadTexture("star.tga", true);
	state->texture[T_MISSILE_1] 	= loadTexture("missile_1.tga", false);
	state->texture[T_EXPLOSION_1] 	= loadTexture("explosion_1.tga", false);
	state->texture[T_EXPLOSION_2] 	= loadTexture("explosion_2.tga", false);
	state->texture[T_EXPLOSION_3] 	= loadTexture("star.tga", false);
	state->texture[T_EXPLOSION_4] 	= loadTexture("explosion_3.tga", false);
	state->texture[T_JET]	 		= loadTexture("star.tga", false);

	if (state->vid_cfg_lowquality) {
		state->texture[T_BACKGROUND_1] = loadTexture("background_1_lq.tga", false);
	} else {
		state->texture[T_BACKGROUND_1] = loadTexture("background_1.tga", false);
	}

	// asteroid
	state->log("Loading 'obj/asteroid_1.obj'\n");
	  asteroid->textures[0] = loadTexture("asteroid_1.tga", false);
	  asteroid->load(state->engine_datadir, "asteroid_1.obj");

	// debris
	if (!state->vid_cfg_lowquality) {
		state->log("Loading 'obj/debris_1.obj'\n");
		  debris->textures[0] = loadTexture("debris_1.tga", false);
		  debris->load(state->engine_datadir, "debris_1.obj");
	}

	// cargo
	state->log("Loading 'obj/cargo_1.obj'\n");
	  cargo->textures[0] = loadTexture("cargo_1.tga", false);
	  cargo->load(state->engine_datadir, "cargo_1.obj");
	  cargo->textures[1] = loadTexture("glow_1.tga", false);

	// player
	state->log("Loading 'obj/ship_1.obj'\n");
	  player->textures[0] = loadTexture("ship_1_1.tga", false);
	  player->textures[1] = loadTexture("ship_1_2.tga", false);
	  player->textures[2] = cargo->textures[1];
	  player->textures[3] = state->texture[T_JET];
	  player->load(state->engine_datadir, "ship_1.obj");

	// powerup
	powerup->textures[0] = cargo->textures[1];

	// samples and music
	state->audio->sample[0] = state->audio->loadSample("menu_1.wav");
	state->audio->sample[1] = state->audio->loadSample("menu_2.wav");
	state->audio->sample[2] = state->audio->loadSample("gun_1.wav");
	state->audio->sample[3] = state->audio->loadSample("engine_1.wav");
	state->audio->sample[4] = state->audio->loadSample("explosion_1.wav");
	state->audio->sample[5] = state->audio->loadSample("explosion_2.wav");
	state->audio->sample[6] = state->audio->loadSample("explosion_3.wav");
	state->audio->sample[7] = state->audio->loadSample("shield_1.wav");
	state->audio->sample[8] = state->audio->loadSample("engine_2.wav");
	state->audio->sample[9] = state->audio->loadSample("powerup_1.wav");
	state->audio->sample[10] = state->audio->loadSample("engine_3.wav");
	state->audio->sample[11] = state->audio->loadSample("logo.wav");

	state->audio->music[0] = state->audio->loadMusic("music_title.ogg");
}

/*
 * draw text
 */
void Scenery::drawText(const char *text, float x, float y, float z, float size, float r, float g, float b, float a) {
	int strLenth = strlen(text);
	char letter, lastletter = 0;
	GLfloat x1 = .0f, w, y1, y2, wy;

	glColor4f(r, g, b, a);
	glBindTexture(GL_TEXTURE_2D, state->texture[T_FONT]);

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
			if (letter <= 58) {				// 0..9, :, /, +
				y1 = .59f;
			} else if (letter >=97) {			// a..z
				y1 = .335f;
			} else {					// A..Z
				y1 = .004f;
			}
			y2 = y1 + .25f;

			switch(letter) {
				case 34:  x1 = 0.435f; w = 0.0170f; break;		// "
				case 36:  x1 = 0.600f; w = 0.0285f; break;		// $
				case 43:  x1 = 0.558f; w = 0.0200f; wy = .03f; break; // +
				case 45:  x1 = 0.509f; w = 0.0161f; wy = .03f; break; // -
				case 46:  x1 = 0.480f; w = 0.0120f; break;		// .
				case 47:  x1 = 0.384f; w = 0.0174f; break;		// /
				case 48:  x1 = 0.300f; w = 0.0320f; break;		// 0
				case 49:  x1 = 0.008f; w = 0.0130f; break;		// 1
				case 50:  x1 = 0.026f; w = 0.0274f; break;		// 2
				case 51:  x1 = 0.062f; w = 0.0255f; break;		// 3
				case 52:  x1 = 0.093f; w = 0.0320f; break;		// 4
				case 53:  x1 = 0.126f; w = 0.0286f; break;		// 5
				case 54:  x1 = 0.159f; w = 0.0305f; break;		// 6
				case 55:  x1 = 0.196f; w = 0.0230f; break;		// 7
				case 56:  x1 = 0.226f; w = 0.0320f; break;		// 8
				case 57:  x1 = 0.264f; w = 0.0310f; break;		// 9
				case 58:  x1 = 0.495f; w = 0.0120f; wy = .06f; break; // :
				case 65:  x1 = 0.006f; w = 0.0415f; break;		// A
				case 66:  x1 = 0.050f; w = 0.0310f; break;		// B
				case 67:  x1 = 0.086f; w = 0.0285f; break;		// C
				case 68:  x1 = 0.121f; w = 0.0370f; break;		// D
				case 69:  x1 = 0.163f; w = 0.0278f; break;		// E
				case 70:  x1 = 0.195f; w = 0.0266f; break;		// F
				case 71:  x1 = 0.230f; w = 0.0335f; break;		// G
				case 72:  x1 = 0.270f; w = 0.0342f; break;		// H
				case 73:  x1 = 0.308f; w = 0.0066f; break;		// I
				case 74:  x1 = 0.320f; w = 0.0200f; break;		// J
				case 75:  x1 = 0.340f; w = 0.0325f; break;		// K
				case 76:  x1 = 0.382f; w = 0.0274f; break;		// L
				case 77:  x1 = 0.413f; w = 0.0418f; break;		// M
				case 78:  x1 = 0.460f; w = 0.0353f; break;		// N
				case 79:  x1 = 0.500f; w = 0.0362f; break;		// O
				case 80:  x1 = 0.540f; w = 0.0302f; break;		// P
				case 81:  x1 = 0.576f; w = 0.0364f; break;		// Q
				case 82:  x1 = 0.617f; w = 0.0316f; break;		// R
				case 83:  x1 = 0.654f; w = 0.0364f; break;		// S
				case 84:  x1 = 0.694f; w = 0.0322f; break;		// T
				case 85:  x1 = 0.730f; w = 0.0356f; break;		// U
				case 86:  x1 = 0.768f; w = 0.0410f; break;		// V
				case 87:  x1 = 0.812f; w = 0.0555f; break;		// W
				case 88:  x1 = 0.870f; w = 0.0380f; break;		// X
				case 89:  x1 = 0.910f; w = 0.0420f; break;		// Y
				case 90:  x1 = 0.954f; w = 0.0376f; break;		// Z
				case 120: x1 = 0.738f; w = 0.0295f; break;		// x
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
void Scenery::drawTextA(const char *text, float x, float y, float z, float size, float r, float g, float b, float a) {
	drawText(text, (-5.97f*state->vid_cfg_aspect)+x, 5.69f-y, z, size, r, g, b, a);
}

/*
 * display frame rate
 */
void Scenery::drawFPS() {
	static char txt[16];

	state->fps_frame++;

	if (state->fps_frame >= state->fps_count) {
		int t = glutGet((GLenum)GLUT_ELAPSED_TIME);

		state->fps_dt = t - state->fps_lt;
		state->fps_lt = t;

		if (state->fps_ready) {
			state->fps = float(state->fps_frame) / (float(state->fps_dt) * .001f);
		}

		state->fps_count = int(round(state->fps));
		state->fps_ready = true;
		state->fps_frame = 0;
	}

	glLoadIdentity();

	float x = -.45f;
	drawTextA("FPS:", x, -.4f, -10.0f, 65, .6f, .6f, .6f, 1.0f);
	if ((state->fps > .0f) && state->fps_ready) {
		sprintf(txt, "%d", int(round(state->fps)));
		drawTextA(txt, x+.65f, -.4f, -10.0f, 65, .6f, .6f, .6f, 1.0f);
	}

	if (state->engine_debug && (state->get() == STATE_GAME_LOOP)) {
		x += 1.7f;
		sprintf(txt, "%d", (int)state->lvl_pos);
		drawTextA("POS:", x, -.4f, -10.0f, 65, .6f, .6f, .6f, 1.0f);
		drawTextA(txt, x+.7f, -.4f, -10.0f, 65, .6f, .6f, .6f, 1.0f);

		x += 1.7f;
		sprintf(txt, "%d", (int)state->lvl_entities);
		drawTextA("OBJ:", x, -.4f, -10.0f, 65, .6f, .6f, .6f, 1.0f);
		drawTextA(txt, x+.6f, -.4f, -10.0f, 65, .6f, .6f, .6f, 1.0f);
	}
}

/*
 * draw intro logo
 */
void Scenery::drawIntro() {
	static float a = 0;
	static float a1 = 0;
	static float z = 0;

	z += state->timer_adjustment * .005f;
	if (a1 < 1.0f) {
		if (z > .0f) {
			a1 += (1.4f - a1) * (state->timer_adjustment * .008f);
			a = a1;
		}
	} else {
		a1 += (2.1f - a1) * (state->timer_adjustment * .01f);
		a -= a1 * (state->timer_adjustment * .02f);
		if (a <= .0f) state->set(STATE_MENU);
	}

	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, state->texture[T_LOGO]);

	glPushMatrix();
	glTranslatef(0, 0, -150.0f + z * 35.0f);
	glColor4f(a, a, a, 1.0f);
	glBegin (GL_QUADS);
	  glTexCoord2f (0, 1);
	  glVertex3f (-75.0f, -20.0f, 0);

	  glTexCoord2f (1, 1);
	  glVertex3f (75.0f, -20.0f, 0);

	  glTexCoord2f (1, 0);
	  glVertex3f (75.0f, 20.0f, 0);

	  glTexCoord2f (0, 0);
	  glVertex3f (-75.0f, 20.0f, 0);
	glEnd();
	glPopMatrix();
}

/*
 * draw title
 */
void Scenery::drawTitle() {
	float sc, a = state->title_ypos * .01f;

	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, state->texture[T_TITLE]);
	glPushMatrix();
	if (state->get() == STATE_MENU) {
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
	if (state->get() == STATE_MENU) {
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
}

/*
 * draw menu
 */
void Scenery::drawMenu(bool mouse_recheck) {
	int i, numentries;
	float mx, my, mh, mf, mo;
	float m_a = state->global_alpha;

	char *mtxt[5];
	for (i=0; i<5; i++) {
		mtxt[i] = (char*)malloc(sizeof(char)*64);
	}

	switch(state->menu) {
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

			if (state->menu_selected) {
				switch (state->menu_pos) {
					case 0: // launch
						state->lvl_id = 1;
						state->set(STATE_GAME_START);
						break;

					case 1: // enter settings
						if (state->menu != 2) {
							state->menu = 2;
							state->menu_pos = 0;
							state->menu_selected = false;
							drawMenu(false);
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
			mx = -1.26f;
			my = 0.015f;
			mh = .75f;
			mf = 105;
			mo = .175f;

			strcpy(mtxt[0], "VIDEO");
			strcpy(mtxt[1], "AUDIO");
			strcpy(mtxt[2], "CANCEL");
			strcpy(mtxt[3], "ACCEPT");

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
						state->config.aud_sfx = state->audio->volume_sfx;
						state->config.aud_music = state->audio->volume_music;
						state->config.aud_mixfreq = state->audio->mixer_frequency;
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

					case 3: // accept
						state->menu = 1;
						state->menu_pos = 1;
						state->engine_restart = true;
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

			sprintf(mtxt[0], "VIDEO MODE:\n     %dX%d",
			  state->vid_sup_modes[state->vid_cfg_mode]->w,
			  state->vid_sup_modes[state->vid_cfg_mode]->h);

			switch(state->config.vid_aspect) {
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

			if (state->config.vid_fullscreen)
				strcpy(mtxt[2], "FULL SCREEN:\n     ENABLED");
			else
				strcpy(mtxt[2], "FULL SCREEN:\n     DISABLED");

			if (state->config.vid_vsync)
				strcpy(mtxt[3], "VERTICAL SYNC:\n     ENABLED");
			else
				strcpy(mtxt[3], "VERTICAL SYNC:\n     DISABLED");

			strcpy(mtxt[4], "RETURN");

			if (state->menu_selected) {
				switch (state->menu_pos) {
					case 0: // toggle video mode
						state->vid_cfg_mode--;
						if (state->vid_cfg_mode < 0)
							state->vid_cfg_mode = state->vid_sup_modes_count-1;
						while ( (state->vid_sup_modes[state->vid_cfg_mode]->w < 800) ||
							(state->vid_sup_modes[state->vid_cfg_mode]->h < 600) ) {
							state->vid_cfg_mode--;
							if (state->vid_cfg_mode < 0)
								state->vid_cfg_mode = state->vid_sup_modes_count-1;
							}
						break;

					case 1: // toggle aspect ration
						state->config.vid_aspect++;
						if (state->config.vid_aspect > 3) {
							state->config.vid_aspect = 0;
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
			mx = -1.26f;
			my = 0.015f;
			mh = .75f;
			mf = 75;
			mo = .375f;

			switch(state->config.aud_sfx) {
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

			switch(state->config.aud_music) {
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

			sprintf(mtxt[2], "MIXER QUALITY:\n     %d HZ", state->config.aud_mixfreq);
			strcpy(mtxt[3], "RETURN");

			if (state->menu_selected) {
				switch (state->menu_pos) {
					case 0: // toggle SFX volume
						if (state->config.aud_sfx != -1) {
							state->config.aud_sfx++;
							if (state->config.aud_sfx > 3)
								state->config.aud_sfx = 0;
						}
						break;

					case 1: // toggle music volume
						if (state->config.aud_music != -1) {
							state->config.aud_music++;
							if (state->config.aud_music > 3)
								state->config.aud_music = 0;
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
		drawMenu(state->mouse_button & SDL_BUTTON(1));
		return;
	}

	// check mouse position
	if (state->mouse_moved || (state->mouse_button & SDL_BUTTON(1)) || mouse_recheck) {
		for (i=0; i<numentries; i++) {

			if ( (state->mouse_y <= .285f - (float(i)   * mh * .505f)) &&
			     (state->mouse_y >  .285f - (float(i+1) * mh * .505f)) &&
			     (state->mouse_x > -2.48f) &&
			     (state->mouse_x < -.55f) ) {

				state->menu_pos = i;

				// mouse pressed?
				if (state->mouse_pressed && !mouse_recheck) {
					state->menu_selected = true;
					state->mouse_pressed = false;

					if ( (state->menu == 1 && state->menu_pos == 2) ||
						 (state->menu == 2 && state->menu_pos == 2) ) {
						state->audio->playSample(1, 128, 0);
					} else {
						state->audio->playSample(0, 128, 0);
					}

					drawMenu(true);
					return;
				}
			}
		}
	}

	if (state->menu_pos >= numentries)
		state->menu_pos = numentries-1;

	if (state->menu_pos < 0)
		state->menu_pos = 0;

	if (state->get() >= STATE_GAME_START) {
		m_a = (float)state->title_ypos;
	}

	// draw player's ship
	player->setPos(state->player, 4.3f, -1.0f, -3.0f);
	player->setRot(state->player, 20.0f, 0, player->getRotZ(state->player) - (state->timer_adjustment * .2f));
	player->draw();

	// draw menu background
	glLoadIdentity();
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, state->texture[T_MENU_1]);
	glTranslatef(0, -0.8f, -10);
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
	drawTextA(PACKAGE_VERSION, -.45f, 12.0f, -10.0f, 65, .85f, .85f, .85f, .0035f*m_a);

	// draw menu items
	for (i=0; i<numentries; i++) {
		drawText(mtxt[i], mx-3.0f, my-(i*mh)+mo, -9.9f, mf, 1, .8f, .55f, .0085f*m_a);

		if (i == state->menu_pos) {
			glLoadIdentity();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, state->texture[T_MENU_2]);
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

		free(mtxt[i]);
	}
}

/*
 *	draw background and stars
 */
void Scenery::drawBackground() {
	unsigned short i;
	float a, c, sl, sa;

	glShadeModel(GL_FLAT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_CULL_FACE);

	// background
	glBindTexture(GL_TEXTURE_2D, state->texture[T_BACKGROUND_1]);
	glRotatef(state->stars_rotation_pos, 0, 0, 1);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor4f(.65f, .7f, .8f, state->global_alpha * .01f);
	glBegin (GL_QUADS);
	  glTexCoord2i (0, 0);
	  glVertex2i (-260, 260);

	  glTexCoord2i (1, 0);
	  glVertex2i (260, 260);

	  glTexCoord2i (1, 1);
	  glVertex2i (260, -260);

	  glTexCoord2i (0, 1);
	  glVertex2i (-260, -260);
	glEnd();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, state->texture[T_STAR]);

	// far stars
	for (i=0; i<(state->engine_stars-state->engine_stars_warp); ++i) {
		c = stars[i][3];
		a = ((500.0f+stars[i][2])/250.0f) * (.0075f * state->global_alpha);

		glPushMatrix();
		glTranslatef(stars[i][0], stars[i][1], stars[i][2]);

		glColor4f(c, c, c, a*.75f);
		c = .75f + c*1.5f;
		glBegin(GL_QUADS);
		  glTexCoord2i(0, 0);
		  glVertex3f(-c, -c, 0);

		  glTexCoord2i(1, 0);
		  glVertex3f(c, -c, 0);

		  glTexCoord2i(1, 1);
		  glVertex3f(c, c, 0);

		  glTexCoord2i(0, 1);
		  glVertex3f(-c, c, 0);
		glEnd();

		glPopMatrix();
	}

	// warp stars
	if (state->stars_warp) {

		for (i=(state->engine_stars - state->engine_stars_warp); i<state->engine_stars; ++i) {
			c = stars[i][3];
			a = ((500.0f+stars[i][2])/250.0f) * (.0075f * state->global_alpha);
			sl = (a*3.5f) * (a*3.5f) * (state->stars_speed - .3f);
			sa = .6f * (state->stars_speed - .3f);

			glPushMatrix();
			glTranslatef(stars[i][0], stars[i][1], stars[i][2]);

			glBegin (GL_QUADS);
			  glColor4f(a, a, a, sa);

			  glTexCoord2i(1, 0);
			  glVertex3f(-1, -1, -sl);

			  glTexCoord2i(1, 1);
			  glVertex3f (1, -1, -sl);

			  glColor4f(c*a, c*a, c*a, .35f*sa);

			  glTexCoord2i(0, 1);
			  glVertex3f (1, 1, sl);

			  glTexCoord2i(0, 0);
			  glVertex3f (-1, 1, sl);
			glEnd();
			glPopMatrix();
		}

	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
}

/*
 * draw mouse cursor
 */
void Scenery::drawMouse() {
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, state->texture[T_CURSOR]);
	glColor4f(1, 1, 1, .01f*state->global_alpha);

	glPushMatrix();
	glTranslatef(state->mouse_x, state->mouse_y, -5.0f);
	glBegin (GL_QUADS);
	  glTexCoord2f (0, 0);
	  glVertex3f (-.1f, .1f, 0);

	  glTexCoord2f (1, 0);
	  glVertex3f (.1f, .1f, 0);

	  glTexCoord2f (1, 1);
	  glVertex3f (.1f, -.1f, 0);

	  glTexCoord2f (0, 1);
	  glVertex3f (-.1f, -.1f, 0);
	glEnd();
	glPopMatrix();
}

/*
 * draw objects
 */
void Scenery::drawObjects() {
	int i;
	float alpha;

	state->objects[state->player].target = -1;
	for (i=0; i<state->lvl_entities; i++) {
		if ( (state->objects[i].waiting == 0) &&
			 (state->objects[i].type == OBJ_TYPE_COLLIDER) ||
			 (state->objects[i].type == OBJ_TYPE_POWERUP) ) {
			player->getTarget(i);
		}
	}

	for (i=0; i<state->lvl_entities; i++) {
		if (state->objects[i].waiting) continue;

		switch(state->objects[i].id) {
			case OBJ_PLAYER:
				player->draw();
				break;
			case OBJ_ASTEROID_1:
				asteroid->draw(i);
				break;
			case OBJ_DEBRIS_1:
				if (!state->vid_cfg_lowquality) debris->draw(i);
				break;
			case OBJ_CARGO_1:
				cargo->draw(i);
				break;
			case OBJ_POWERUP_1:
				powerup->draw(i);
				object->drawCrosshair(i, .3f, .55f, 1.0f);
				break;
			case OBJ_EXPLOSION_1:
				glLoadIdentity();
				glBindTexture(GL_TEXTURE_2D, state->texture[T_EXPLOSION_1]);
				explosion->draw(i);
				break;
			case OBJ_EXPLOSION_2:
				glLoadIdentity();
				glBindTexture(GL_TEXTURE_2D, state->texture[T_EXPLOSION_2]);
				explosion->draw(i);
				break;
			case OBJ_EXPLOSION_3:
				glLoadIdentity();
				glBindTexture(GL_TEXTURE_2D, state->texture[T_EXPLOSION_2]);
				explosion->draw(i);
				break;
			case OBJ_EXPLOSION_4:
				glLoadIdentity();
				glBindTexture(GL_TEXTURE_2D, state->texture[T_STAR]);
				explosion->draw(i);
				break;
			case OBJ_EXPLOSION_5:
				glLoadIdentity();
				glBindTexture(GL_TEXTURE_2D, state->texture[T_EXPLOSION_4]);
				explosion->draw(i);
				break;
			case OBJ_MISSILE_1:
				glLoadIdentity();
				glBindTexture(GL_TEXTURE_2D, state->texture[T_MISSILE_1]);

				alpha = (state->global_alpha * .005f) + ((state->objects[i].pos_z+200.0f)*.00002f);
				if (alpha < 0) {
					state->remove(i);
					break;
				}

				glColor4f(.5f, 1.0f, .8f, alpha);
				glPushMatrix();
				glTranslatef(
				  (state->objects[i].pos_x - state->cam_x) * E_RELATIVE_MOVEMENT,
				  (state->objects[i].pos_y - state->cam_y) * E_RELATIVE_MOVEMENT,
				  state->objects[i].pos_z
				);
				glRotatef(270, 0, 0, 1);
				glScalef(state->objects[i].scale_x, state->objects[i].scale_y, state->objects[i].scale_z);
				glBegin (GL_QUADS);
				  glTexCoord2f (0, 0);
				  glVertex3f (-3.0f, 0, 0);

				  glTexCoord2f (1, 0);
				  glVertex3f (3.0f, 0, 0);

				  glTexCoord2f (1, .95f);
				  glVertex3f (3.0f, 0, 150.0f);

				  glTexCoord2f (0, .95f);
				  glVertex3f (-3.0f, 0, 150.0f);
				glEnd();
				glRotatef(90, 0, 0, 1);
				glBegin (GL_QUADS);
				  glTexCoord2f (0, 0);
				  glVertex3f (-3.0f, 0, 0);

				  glTexCoord2f (1, 0);
				  glVertex3f (3.0f, 0, 0);

				  glTexCoord2f (1, .95f);
				  glVertex3f (3.0f, 0, 150.0f);

				  glTexCoord2f (0, .95f);
				  glVertex3f (-3.0f, 0, 150.0f);
				glEnd();
				glPopMatrix();
				break;
		}
	}
}

/*
	Draw HUD (energy, shields, money)
*/
void Scenery::drawHUD() {
	int i, s, e;
	static float alpha = 1.0f;
	float t;
	char msg[16];

	switch(state->get()) {
		case STATE_GAME_START:
			t = (-6.413f * state->vid_cfg_aspect - state->hud_x) * .055f * state->timer_adjustment;
			state->hud_x += t;
			t = (4.905f + state->hud_y) * .055f * state->timer_adjustment;
			state->hud_y -= t;
			alpha = (100-state->title_ypos)*.01f;
			break;
		case STATE_GAME_NEXTLEVEL:
		case STATE_GAME_QUIT:
			if (state->title_ypos > 0) {
				state->hud_x -= state->title_ypos * .001f;
				state->hud_y -= state->title_ypos * .001f;
			}
			alpha = (100-state->title_ypos)*.01f;
			break;
		case STATE_GAME_LOOP:
			if (state->objects[state->player].life <= 0) {
				state->hud_x -= state->timer_adjustment * .01f;
				state->hud_y -= state->timer_adjustment * .01f;
				alpha -= state->timer_adjustment * .01f;
			} else {
				state->hud_x = -6.413f * state->vid_cfg_aspect;
				state->hud_y = -4.905f;
				alpha = (100.0f-float(state->title_ypos))*.01f;
			}
			break;
		default:
			state->hud_x = .0f;
			state->hud_y = -.8f;
			alpha = (100.0f-float(state->title_ypos))*.01f;
	}

	glLoadIdentity();

	// lower right screen
	glBindTexture(GL_TEXTURE_2D, state->texture[T_MENU_1]);
	glPushMatrix();
	glTranslatef(-state->hud_x - state->tilt_x * .01f, state->hud_y - state->tilt_y * .01f, -10);
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
	sprintf(msg, "%d $", state->objects[state->player].money);
	drawText(msg, -4.6f, -1.2f, 0, 130, 1.0f, 1.0f, .3f, alpha * .85f);
	glPopMatrix();

	glLoadIdentity();

	// lower left screen
	glBindTexture(GL_TEXTURE_2D, state->texture[T_MENU_1]);
	glPushMatrix();
	glTranslatef(state->hud_x - state->tilt_x * .01f, state->hud_y - state->tilt_y * .01f, -10);
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

	// life symbol
	glBindTexture(GL_TEXTURE_2D, state->texture[T_HUD_1]);
	glPushMatrix();
	glTranslatef(state->hud_x + .5f - state->tilt_x*.01f, state->hud_y - .7f - state->tilt_y*.01f, -9.9f);
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
	s = int(50.0f / ( (float(player->shield_max + 1) / float(state->objects[state->player].life + 1)) ));
	for (i=0; i<s; i++) {
		drawText("I", .32f+.077f*i, -.15f, 0, 80, 1.0f, .4f, .2f, .85f*(1.0f-(.02f*((s+1)-i)))*alpha);
	}

	// energy symbol
	glBindTexture(GL_TEXTURE_2D, state->texture[T_HUD_2]);
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
	e = int(50.0f/(((float)player->energy_max+1) / (float(state->objects[state->player].energy+1))));
	for (i=0; i<e; i++) {
		drawText("I", .32f+.077f*i, -.11f, 0, 80, .2f, .65f, 1.0f, .85f*(1.0f-(.02f*((e+1)-i)))*alpha);
	}
	glPopMatrix();
}

void Scenery::drawMessages() {
	int i;
	float r = 1.0f, g = .8f, b = .55f;
	float x, y, z;

	glLoadIdentity();
	glPushMatrix();
	glTranslatef(
		.0f, -1.75f, -10.0f
	);

	for (i=0; i<state->msg_num; i++) {

		switch(state->msg[i].type) {

			case MSG_DAMAGE:
				r = 1.0f;
				g = .25f + state->msg[i].counter * .0055f;
				b = .15f + state->msg[i].counter * .004f;
				break;

			case MSG_MONEY:
				r = 1.0f;
				g = 1.0f - state->msg[i].counter * .002f;
				b = .3f + state->msg[i].counter * .0025f;
				break;

			case MSG_ENERGY:
				r = .3f + state->msg[i].counter * .0025f;
				g = .75f - state->msg[i].counter * .002f;
				b = 1.0f;
				break;

			default:
				continue;

		}

		if (state->msg[i].direction_x > 0) {
			x = state->msg[i].direction_x * (state->msg[i].counter * state->msg[i].counter * state->vid_cfg_aspect * .0003f);
		} else {
			x = state->msg[i].direction_x * (state->msg[i].counter * state->msg[i].counter * state->vid_cfg_aspect * .0004f);
		}
		x += state->msg[i].direction_x * 1.0f;
		y = -1.5f + state->msg[i].direction_y * (state->msg[i].counter * .0265f);
		z = state->msg[i].counter * .01f;

		drawText(
			state->msg[i].text,
			x, y, z,
			75.0f + state->msg[i].counter * .75f,
			r, g, b,
			1.0f - state->msg[i].counter * .01f
		);
	}

	glPopMatrix();
}

/*
 * move objects
 */
void Scenery::moveObjects() {
	int i, j, dmg;
	int sangle = 0;
	float dx, dy, dz, da, dd, ix, iy, s;
	float mx, my, mz, ox, oy, oz;
	static GLuint nextdebris = state->timer;

	if (!state->vid_cfg_lowquality) {
		if ((state->timer > nextdebris) && (state->lvl_pos < float(state->lvl_length-1000))) {
			nextdebris = state->timer + 150 + rand() % 150;

			object_t new_debris;

			new_debris.type			= OBJ_TYPE_SCENERY;
			new_debris.id			= OBJ_DEBRIS_1;
			new_debris.waiting		= false;

			new_debris.life			= -1;
			new_debris.life_max		= -1;
			new_debris.life_time	= -1;
			new_debris.cnt			= .0f;
			new_debris.speed		= .0f;

			new_debris.pos_x		= -600.0f + float(rand() % 1200);
			new_debris.pos_y		= -400.0f + float(rand() % 800);
			new_debris.pos_z		= -8000.0f;

			new_debris.rot_x		= .0f;
			new_debris.rot_y		= .0f;
			new_debris.rot_z		= .0f;

			new_debris.rsp_x		= float(rand() % 100) * .1f;
			new_debris.rsp_y		= float(rand() % 100) * .1f;
			new_debris.rsp_z		= float(rand() % 100) * .1f;

			new_debris.scale_x		= 25.0f + float(rand() % 500) * .2f;
			new_debris.scale_y		= 25.0f + float(rand() % 500) * .2f;
			new_debris.scale_z		= 25.0f + float(rand() % 500) * .2f;

			state->add(&new_debris);
		}
	}

	explosion->move(-1);
	powerup->move(-1);

	state->audio->updatePosition(state->objects[state->player].pos_x - state->cam_x);

	for (i=0; i<state->lvl_entities; i++) {

		// activate waiting object?
		if (state->objects[i].waiting) {
			if (state->lvl_pos > state->objects[i].pos_z) {
				state->objects[i].waiting = false;
				state->objects[i].pos_z = -9999.0f;
			} else {
				continue;
			}
		}

		// move object
		switch (state->objects[i].id) {
			case OBJ_PLAYER:
				player->move();
				break;

			case OBJ_EXPLOSION_1:
			case OBJ_EXPLOSION_2:
			case OBJ_EXPLOSION_3:
			case OBJ_EXPLOSION_4:
			case OBJ_EXPLOSION_5:
				explosion->move(i);
				break;

			case OBJ_MISSILE_1:
				state->objects[i].pos_x += state->timer_adjustment * state->objects[i].rsp_x;
				state->objects[i].pos_y += state->timer_adjustment * state->objects[i].rsp_y;
				state->objects[i].pos_z += state->timer_adjustment * state->objects[i].speed;
				if (state->objects[i].pos_z < -10000.0f) {
					state->remove(i);
					continue;
				}
				break;

			default:
				object->move(i);
		}

		// collision detection
		switch (state->objects[i].type) {

			// missiles are checked against colliders and obstacles
			case OBJ_TYPE_MISSILE:
				for (j=0; j<state->lvl_entities; j++) {

					// missiles only hit colliders
					if ( (state->objects[j].waiting) ||
					     (state->objects[j].type != OBJ_TYPE_COLLIDER) ) continue;

					// colliders have already passed by
					if (state->objects[i].pos_z < state->objects[j].pos_z) continue;

					// missile's position in next frame
					mx = state->objects[i].pos_x;
					my = state->objects[i].pos_y;
					mz = state->objects[i].pos_z + (state->objects[i].speed * state->timer_adjustment);

					// object's position in next frame
					ox = state->objects[j].pos_x;
					oy = state->objects[j].pos_y;
					oz = state->objects[j].pos_z + (state->objects[j].speed * state->timer_adjustment);

					// distance between object and missile in next frame
					dx = fabs(mx - ox);
					dy = fabs(my - oy);
					dz = mz - oz;

					// object's size
					s = ((10000.0f + oz) * .0001f) /
						   isqrt( state->objects[j].scale_x * state->objects[j].scale_x +
								  state->objects[j].scale_y * state->objects[j].scale_y +
								  state->objects[j].scale_z * state->objects[j].scale_z
							);

					if ( (dx < s*.5f) && (dy < s*.5f) && (dz < s) ) {

						if (state->objects[i].pos_x < 0) {
							sangle = 360 + (int)(state->objects[j].pos_x / 2.8f);
						} else {
							sangle = (int)(state->objects[j].pos_x / 2.8f);
						}
						state->audio->playSample(4, 255, sangle);

						state->objects[j].life -= player->gun_power;

						if (state->objects[j].life > 0) {
							// gun impact
							explosion->add(
								OBJ_EXPLOSION_1,
								state->objects[i].pos_x,
								state->objects[i].pos_y,
								state->objects[j].pos_z + s,
								state->objects[j].speed
							);
						} else {
							if (state->objects[j].id == OBJ_ASTEROID_1) {
								// smoke
								explosion->add(
									OBJ_EXPLOSION_2,
									state->objects[j].pos_x,
									state->objects[j].pos_y,
									state->objects[j].pos_z + .2f,
									state->objects[j].speed
								);
							} else {
								explosion->add(
									// nova
									OBJ_EXPLOSION_5,
									state->objects[j].pos_x,
									state->objects[j].pos_y,
									state->objects[j].pos_z,
									state->objects[j].speed
								);
							}

							explosion->add(
								OBJ_EXPLOSION_3,
								state->objects[j].pos_x,
								state->objects[j].pos_y,
								state->objects[j].pos_z,
								state->objects[j].speed
							);

							// nearby explosion causes impulse and camera tilt
							dz = fabs(state->objects[state->player].pos_z - state->objects[j].pos_z - s);
							dd = 1.0f / isqrt(dx*dx + dy*dy + dz*dz);
							if (dd < 2000.0f) {
								player->tilt((2000.0f - dd) * .01f);

								da = atan(dy/dx);
								dd *= .0002f;

								// calculate horizontal crash impulse
								ix = dd * cos(da);
								if (ix > .25f) ix = .25f;
								if (state->objects[state->player].pos_x > state->objects[i].pos_x) {
									ix *= -1.0f;
								}

								// calculate vertical crash impulse
								iy = dd * sin(da);
								if (iy > .25f) iy = .25f;
								if (state->objects[state->player].pos_y > state->objects[i].pos_y) {
									iy *= -1.0f;
								}

								// transmit impulse to ship
								player->impulse(state->player, ix, iy, .0f);

								dmg = 20 - int((fabs(ix)+fabs(iy))*40.0f);
								if (dmg > 0) {
									state->addMessage(dmg, MSG_DAMAGE);
									state->objects[state->player].life -= dmg;
									if (state->objects[state->player].life <= 0) {
										player->setSpin(state->player, ix, iy, 0);
										state->audio->stopSampleLoop(1000);
										if (state->config.aud_music > 0) state->audio->stopMusic(5000);
									}
								}
							}

							state->explode(j);
						}

						// remove missile after collision
						state->remove(i);
						break;
					}
				}
				break;

			// check player's ship against colliding objects
			case OBJ_TYPE_COLLIDER:
			case OBJ_TYPE_POWERUP:
				// players's position in next frame
				mx = state->objects[state->player].pos_x;
				my = state->objects[state->player].pos_y;
				mz = state->objects[state->player].pos_z - (state->objects[state->player].speed * state->timer_adjustment);

				// object's position in next frame
				ox = state->objects[i].pos_x;
				oy = state->objects[i].pos_y;
				oz = state->objects[i].pos_z + (state->objects[i].speed * state->timer_adjustment);

				// distance between player and object in next frame
				dx = fabs(mx - ox);
				dy = fabs(my - oy);
				dz = fabs(mz - oz);

				// object's size
				s = ((10000.0f + oz) * .0000625f) /
						isqrt( state->objects[i].scale_x * state->objects[i].scale_x +
							   state->objects[i].scale_y * state->objects[i].scale_y +
							   state->objects[i].scale_z * state->objects[i].scale_z );

				if ( (dx < s) && (dy < s) && (dz < s * 1.75f) ) {

					// pick up powerup
					if (state->objects[i].type == OBJ_TYPE_POWERUP) {
						state->explode(i);
						break;
					}

					static float lastcrash = state->timer - 201;
					if ( (state->timer - lastcrash) <= 200) break;
					lastcrash = state->timer;

					// calculate object's absolute distance and angle
					dd = 1.0f / isqrt(dx*dx + dy*dy);
					da = atan(dy/dx);

					// calculate horizontal crash impulse
					ix = (s/dd) * cos(da);
					if (ix > .65f) ix = .65f;
					if (state->objects[state->player].pos_x > state->objects[i].pos_x) {
						ix *= -1.0f;
					}

					// calculate vertical crash impulse
					iy = (s/dd) * sin(da);
					if (iy > .65f) iy = .65f;
					if (state->objects[state->player].pos_y > state->objects[i].pos_y) {
						iy *= -1.0f;
					}

					// transmit impulse to ship and tilt camera
					player->impulse(state->player, ix, iy, 0);
					player->tilt((fabs(ix)+fabs(iy)) * 15.0f );

					// calculate damage
					dmg = 25 + int(float(da)*.75f*float(state->objects[i].life));
					if (dmg > state->objects[i].life) dmg = state->objects[i].life;
					if (dmg == 0) break;

					// collision noise
					state->audio->playSample(7, 192, 180);
					if (state->objects[state->player].life <= 0) break;

					// damage ship & colliding object
					state->objects[i].life -= dmg;
					state->objects[state->player].life -= dmg;
					state->addMessage(dmg, MSG_DAMAGE);

					// colliding object destroyed?
					if (state->objects[i].life < 1) {
						explosion->add(
							OBJ_EXPLOSION_2,
							state->objects[i].pos_x,
							state->objects[i].pos_y,
							state->objects[i].pos_z + .2f,
							state->objects[i].speed
						);

						explosion->add(
							OBJ_EXPLOSION_3,
							state->objects[i].pos_x,
							state->objects[i].pos_y,
							state->objects[i].pos_z,
							state->objects[i].speed
						);

						explosion->add(
							OBJ_EXPLOSION_4,
							state->objects[i].pos_x,
							state->objects[i].pos_y,
							state->objects[i].pos_z + .1f,
							state->objects[i].speed
						);

						state->explode(i);
					} else {
						// collision sparks
						explosion->add(
							OBJ_EXPLOSION_4,
							state->objects[i].pos_x + (state->objects[state->player].pos_x - state->objects[i].pos_x),
							state->objects[i].pos_y + (state->objects[state->player].pos_y - state->objects[i].pos_y),
							state->objects[i].pos_z + (state->objects[state->player].pos_z - state->objects[i].pos_z),
							state->objects[i].speed
						);
					}

					// player's ship was destroyed
					if (state->objects[state->player].life <= 0) {
						state->engine_boundary = false;

						state->audio->playSample(6, 200, 180);
						state->audio->playSample(7, 160, 180);
						state->audio->playSample(8, 255, 180);

						player->setSpin(state->player, ix, iy, 0);
						state->audio->stopSampleLoop(1000);
						state->audio->stopMusic(5000);
					}
				}
				break;

		}
	}

	state->sort();
}

/*
 * move money/damage messages
 */
void Scenery::moveMessages() {
	int i;

	for (i=0; i<state->msg_num; i++) {
		state->msg[i].counter += state->timer_adjustment * (.5f + state->msg[i].counter * .035f);

		if (state->msg[i].counter > 100.0f) {
			state->msg_num--;

			if (i < state->msg_num) {
				memcpy(&state->msg[i], &state->msg[state->msg_num], sizeof(message_t));
				i--;
			}
		}
	}
}

/*
 * move everything
 */
void Scenery::move() {
	int i;

	// move, rotate stars
	for (i=0; i<state->engine_stars; i++) {
		stars[i][2] += state->timer_adjustment * state->stars_speed;
		if (stars[i][2] > 300) stars[i][2] -= 1000;
	}

	if (state->stars_rotation) {
		state->stars_rotation_pos -= state->timer_adjustment * state->stars_rotation_speed;
	}

	switch (state->get()) {
		case STATE_MENU:
			if (state->title_ypos < 99.85f) {
				state->title_ypos += (100.1f-state->title_ypos)*state->timer_adjustment * .025f;
				state->global_alpha = (int)state->title_ypos;
			}
			break;

		case STATE_QUIT:
			if (state->title_ypos > 0) {
				state->title_ypos -= (100.1f-state->title_ypos)*state->timer_adjustment * .15f;
				state->global_alpha = (int)state->title_ypos;
			} else {
				state->set(STATE_CLOSE);
			}
			break;

		case STATE_GAME_START:
			if (state->title_ypos > 0) {
				state->title_ypos -= (100.1f-state->title_ypos)*state->timer_adjustment*.15f;
				state->stars_rotation_speed = .05f * state->title_ypos * .01f;
			}
			if (state->stars_speed > .3f) {
				state->stars_speed -= (state->stars_speed - .2f) * .02f * state->timer_adjustment;
			} else if (state->lvl_loaded) {
				state->set(STATE_GAME_LOOP);
			} else {
				state->set(STATE_QUIT);
			}
			break;

		case STATE_GAME_LOOP:
			state->lvl_pos += state->timer_adjustment * 1.5f;
			if ( (state->lvl_pos > state->lvl_length) &&
			     (state->objects[state->player].life > 0) ) {
				state->set(STATE_GAME_NEXTLEVEL);
			}
			moveObjects();
			moveMessages();

			if (state->lvl_pos < 50) {
				player->accelerateZ(state->player, 17.5f);
			} else {
				if (state->objects[state->player].life > 0) {
					player->accelerateZ(state->player, .0f);
				} else {
					player->accelerateZ(state->player, -4.0f);
				}
			}

			// update chasecam position
			if (state->objects[state->player].life <= 0) {
				if (state->cam_speed > .01f) {
					state->cam_speed -= (state->cam_speed + .015f) * .015f * state->timer_adjustment;
				} else state->cam_speed = 0;
				if (state->objects[state->player].pos_z > .0f) state->set(STATE_GAME_QUIT);
			} else {
				if (state->cam_speed < .5f) {
					state->cam_speed += (.5f - state->cam_speed) * .01f * state->timer_adjustment;
				}
			}
			if (state->cam_speed > 0) {
				state->cam_x += state->timer_adjustment * ( (state->objects[state->player].pos_x * state->cam_speed) - (state->cam_x * state->cam_speed)) * .15f;
				state->cam_y += state->timer_adjustment * ( (state->objects[state->player].pos_y * state->cam_speed) - ((state->cam_y - state->cam_y_offset) * state->cam_speed)) * .175f;
			}
			break;

		case STATE_GAME_NEXTLEVEL:
			moveObjects();
			moveMessages();
			if (state->title_ypos < 99.85f) {
				state->title_ypos += state->timer_adjustment * .375;
				state->cam_y_offset = 35.0f + state->title_ypos * .85f;
				if (state->title_ypos > 80.0f) {
					// jump!
					state->global_alpha = int(99.85f - (state->title_ypos - 80.0f) * 5.0f);
					player->accelerateZ(state->player, 350.0f);
				} else {
					state->global_alpha = 100;
					player->accelerateZ(state->player, state->title_ypos * state->title_ypos * -.0001f);
					player->tilt(state->title_ypos * .05f);
				}
				if (state->stars_speed < 1.75f) {
					state->stars_speed += (state->stars_speed - .1f) * .03f * state->timer_adjustment;
				}
				state->cam_x += state->timer_adjustment * ((state->objects[state->player].pos_x * state->cam_speed) - ((state->tilt_x * 2.0f + state->cam_x) * state->cam_speed)) * .15f;
				state->cam_y += state->timer_adjustment * ((state->objects[state->player].pos_y * state->cam_speed) - ((state->tilt_y * 2.0f + state->cam_y - state->cam_y_offset) * state->cam_speed)) * .15f;
			} else {
				state->set(STATE_MENU);
			}
			break;

		case STATE_GAME_QUIT:
			moveObjects();
			moveMessages();
			if (state->title_ypos < 99.85f) {
				if (state->objects[state->player].life > 0) player->accelerateZ(state->player, -25.0f);
				state->title_ypos += (100.1f - state->title_ypos) * state->timer_adjustment * .075f;
				state->global_alpha = (int)(100.1f - state->title_ypos);
			} else {
				state->set(STATE_MENU);
			}
			break;
	}
}

/*
 * draw everything
 */
void Scenery::draw() {
	float p_x, p_y;

	if ( (state->get() >  STATE_GAME_START) &&
         (state->get() <= STATE_GAME_QUIT) ) {

		p_x = state->objects[state->player].pos_x;
		p_y = state->objects[state->player].pos_y;

		if (p_x < -600.0f) p_x = -600.0f;
		if (p_x >  600.0f) p_x =  600.0f;
		if (p_y < -400.0f) p_y = -400.0f;
		if (p_y >  400.0f) p_y =  400.0f;
	} else {
		p_x = .0f;
		p_y = -90.0f;
	}

	// set up view
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(
	  -p_x*.01f + state->tilt_x*.4f, -p_y*.01f + state->objects[state->player].a_y*10.0f + state->tilt_y*.4f, 200.0f,
	  state->objects[state->player].a_x*.5f, .0f, -10000.0f,
	  state->objects[state->player].a_x*.05f, -1.0f, .0f
	);

	if (state->get() == STATE_INTRO) {
		drawIntro();
		return;
	}

	drawBackground();

	if (state->get() >= STATE_GAME_START) {
		drawObjects();
		drawHUD();
		drawMessages();
	}

	if (state->menu) {
		drawMenu(false);
		drawTitle();
		if (state->get() == STATE_MENU) drawMouse();
	}

	if (state->fps_visible) {
		drawFPS();
	}
}
