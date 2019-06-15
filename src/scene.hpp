#ifndef SCENE_HPP_
#define SCENE_HPP_

using namespace std;

#include <algorithm>
#include <math.h>
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "../config.h"

#include "functions.hpp"
#include "state.hpp"

#include "entity.hpp"
#include "asteroid.hpp"
#include "cargo.hpp"
#include "explosion.hpp"
#include "player.hpp"
#include "powerup.hpp"

class Scene
{
    public:
        Scene(State &s);
        ~Scene();

        shared_ptr<Player> player;

        void               load();
        void               move();
        void               draw();

    private:
        State&             state;

        float              stars[8000][4];

        void               drawText(const char *text, float x, float y, float z, float size, float r, float g, float b, float a);
        void               drawTextA(const char *text, float x, float y, float z, float size, float r, float g, float b, float a);
        void               drawBackground();
        void               drawMouse();
        void               drawFPS();
        void               drawTitle();
        void               drawMenu(bool mouse_recheck);
        void               drawScene();
        void               drawDisplay();
        void               drawMessages();

        void               moveScene();
        void               moveMessages();

        bool               loadLevel();
};

#endif
