#ifndef SCENERY_HPP_
#define SCENERY_HPP_

#include <time.h>
#include <math.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "../config.h"

#include "object.hpp"
#include "player.hpp"
#include "state.hpp"
#include "explosion.hpp"
#include "cargo.hpp"
#include "powerup.hpp"

#define ABS(x) ((x>0)?(x):(-x))

class Scenery {
    public:
        Scenery(State &s);
        ~Scenery();

        Player*         player;

        void            load();
        void            move();
        void            draw();

    protected:
        State&          state;

        Object*         object;
        Object*         asteroid;
        Object*         debris;
        Cargo*          cargo;
        Explosion*      explosion;
        Powerup*        powerup;

        float           stars[8000][4];

        // textures
        GLuint          loadTexture(const char *filename, bool mipmap);

        // draw methods
        void            drawText(const char *text, float x, float y, float z, float size, float r, float g, float b, float a);
        void            drawTextA(const char *text, float x, float y, float z, float size, float r, float g, float b, float a);
        void            drawBackground();
        void            drawMouse();
        void            drawFPS();
        void            drawTitle();
        void            drawMenu(bool mouse_recheck);
        void            drawHUD();
        void            drawMessages();
        void            drawObjects();

        // movement
        void            moveObjects();
        void            moveMessages();
};

#endif /* SCENERY_HPP_ */
