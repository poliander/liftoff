#ifndef SCENE_HPP_
#define SCENE_HPP_

using namespace std;

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "../config.h"

#include "entity.hpp"
#include "object.hpp"
#include "player.hpp"
#include "state.hpp"
#include "explosion.hpp"
#include "cargo.hpp"
#include "powerup.hpp"

class Scene
{
    public:
        Scene(State &s);
        ~Scene();

        void           load();
        void           move();
        void           draw();

        Player*        player = nullptr;

    private:
        State&         state;
        vector<Entity> entities;

        Object*        asteroid = nullptr;
        Object*        debris = nullptr;
        Cargo*         cargo = nullptr;
        Explosion*     explosion = nullptr;
        Powerup*       powerup = nullptr;

        float          stars[8000][4];
        GLuint         loadTexture(const char *filename, bool mipmap);

        void           drawText(const char *text, float x, float y, float z, float size, float r, float g, float b, float a);
        void           drawTextA(const char *text, float x, float y, float z, float size, float r, float g, float b, float a);
        void           drawBackground();
        void           drawMouse();
        void           drawFPS();
        void           drawTitle();
        void           drawMenu(bool mouse_recheck);

        void           drawScene();
        void           drawDisplay();
        void           drawMessages();

        void           moveScene();
        void           moveMessages();
};

#endif