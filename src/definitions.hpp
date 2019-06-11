#ifndef DEFINITIONS_HPP_
#define DEFINITIONS_HPP_

#define DEFAULT_CFG_FILE        "liftoff.cfg"
#define GAME_DATA               "."

#define ABS(x) ((x>0)?(x):(-x))

// video, audio, quality defaults
#define DEFAULT_VID_FULLSCREEN  true
#define DEFAULT_VID_WIDTH       -1
#define DEFAULT_VID_HEIGHT      -1
#define DEFAULT_VID_VSYNC       false
#define DEFAULT_AUD_MIXFREQ     MIX_DEFAULT_FREQUENCY
#define DEFAULT_AUD_FORMAT      MIX_DEFAULT_FORMAT
#define DEFAULT_GFX_STARS       3000
#define DEFAULT_GFX_STARS_WARP  1000
#define DEFAULT_GFX_PARTICLES   100

// engine defaults & fine tuning
#define E_LOG_FILE              false
#define E_RELATIVE_MOVEMENT     2.5
#define E_BASE_SPEED            25.0
#define E_MAX_OBJECTS           1024
#define E_MAX_MESSAGES          64

// engine states
#define STATE_INTRO             -2
#define STATE_QUIT              -1
#define STATE_CLOSE             0
#define STATE_MENU              1
#define STATE_GAME_START        10
#define STATE_GAME_LOOP         11
#define STATE_GAME_NEXTLEVEL    98
#define STATE_GAME_QUIT         99

// object types
#define OBJ_TYPE_PLAYER         1
#define OBJ_TYPE_SCENERY        2
#define OBJ_TYPE_COLLIDER       3
#define OBJ_TYPE_MISSILE        4
#define OBJ_TYPE_EXPLOSION      5

// object states
#define OBJ_STATE_IDLE          0
#define OBJ_STATE_ACTIVE        1
#define OBJ_STATE_FADING        8
#define OBJ_STATE_GONE          9

// object IDs
#define OBJ_PLAYER              101 // player
#define OBJ_CARGO_1             201 // cargo
#define OBJ_ASTEROID_1          301 // asteroid
#define OBJ_DEBRIS_1            311 // space debris
#define OBJ_MISSILE_1           401 // green laser
#define OBJ_EXPLOSION_1         501 // green gun impact
#define OBJ_EXPLOSION_2         502 // explosion smoke
#define OBJ_EXPLOSION_3         503 // explosion fire
#define OBJ_EXPLOSION_4         504 // explosion sparks or collision
#define OBJ_EXPLOSION_5         505 // explosion nova
#define OBJ_POWERUP_0           601 // energy and shield boost
#define OBJ_POWERUP_1           602 // energy boost 150-300 units

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
#define T_EXPLOSION_3           13
#define T_EXPLOSION_4           14
#define T_BACKGROUND_1          15
#define T_JET                   16

// messages
#define MSG_MONEY               1
#define MSG_DAMAGE              2
#define MSG_ENERGY              3
#define MSG_SHIELD              4

#endif
