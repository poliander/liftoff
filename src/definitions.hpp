#pragma once

#define ABS(x) ((x>0)?(x):(-x))

#define DEFAULT_GAMEDATA        "resources"
#define CFG_FILENAME            "liftoff.cfg"

// video/audio defaults
#define DEFAULT_VID_FULLSCREEN  true
#define DEFAULT_VID_WIDTH       -1
#define DEFAULT_VID_HEIGHT      -1
#define DEFAULT_VID_VSYNC       false
#define DEFAULT_AUD_MIXFREQ     MIX_DEFAULT_FREQUENCY
#define DEFAULT_AUD_FORMAT      MIX_DEFAULT_FORMAT
#define DEFAULT_GFX_PARTICLES   100

// engine globals
#define E_RELATIVE_MOVEMENT     2.5
#define E_BASE_SPEED            25.0
#define E_MAX_MESSAGES          64

// engine states
#define STATE_MENU              1
#define STATE_GAME_START        10
#define STATE_GAME_LOOP         11
#define STATE_GAME_NEXTLEVEL    12
#define STATE_GAME_QUIT         13
#define STATE_QUIT              98
#define STATE_CLOSE             99

// entity states
#define E_STATE_IDLE            0
#define E_STATE_ACTIVE          1
#define E_STATE_FADING          8
#define E_STATE_GONE            9

// entity types
#define E_TYPE_PLAYER           1
#define E_TYPE_SCENERY          2
#define E_TYPE_COLLIDER         3

// messages types
#define MSG_MONEY               1
#define MSG_DAMAGE              2
#define MSG_ENERGY              3

// textures
#define T_LOGO                  0
#define T_TITLE                 1
#define T_FONT                  2
#define T_CURSOR                3
#define T_MENU_1                4
#define T_MENU_2                5
#define T_HUD_1                 6
#define T_HUD_2                 7
#define T_HUD_3                 8
#define T_STAR                  9
#define T_MISSILE_1             10
#define T_EXPLOSION_1           11
#define T_EXPLOSION_2           12
#define T_BACKGROUND_1          13
#define T_JET_EXHAUST           14
#define T_GLOW                  15

// objects
#define OBJ_PLAYER              101 // player
#define OBJ_CARGO_1             201 // cargo
#define OBJ_ASTEROID_1          301 // asteroid
#define OBJ_DEBRIS_1            311 // space debris
#define OBJ_MISSILE_1           401 // green laser
#define OBJ_EXPLOSION_1         501 // explosion (gun impact)
#define OBJ_EXPLOSION_2         502 // explosion (smoke)
#define OBJ_EXPLOSION_3         503 // explosion (sparks)
#define OBJ_EXPLOSION_4         504 // explosion (collision sparks)
#define OBJ_EXPLOSION_5         505 // explosion (halo)
#define OBJ_POWERUP_0           601 // energy and life boost at level start
#define OBJ_POWERUP_1           602 // energy and life boost

// sound fx ids
#define SFX_GUN_IMPACT          4
#define SFX_EXPLOSION_1         5
#define SFX_EXPLOSION_2         6
#define SFX_POWERUP_1           9
