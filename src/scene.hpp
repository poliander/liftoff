#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "../config.h"

#include "functions.hpp"
#include "state.hpp"

#include "entity.hpp"
#include "asteroid.hpp"
#include "cargo.hpp"
#include "debris.hpp"
#include "explosion.hpp"
#include "player.hpp"
#include "powerup.hpp"
#include "skybox.hpp"

using namespace std;

class Scene
{
    public:
        Scene(State &s);
        ~Scene();

        void               load();
        void               move();
        void               draw();

    private:
        State&             state;

        shared_ptr<Player> player;
        shared_ptr<Skybox> skybox;

        void               drawText(const char *text, float x, float y, float z, float size, float r, float g, float b, float a);
        void               drawTextA(const char *text, float x, float y, float z, float size, float r, float g, float b, float a);
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
