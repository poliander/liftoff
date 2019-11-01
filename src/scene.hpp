#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include <SDL2/SDL.h>

#include <GL/glew.h>
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
        void               update();
        void               draw();

    private:
        State&             state;

        shared_ptr<Player> player;
        shared_ptr<Skybox> skybox;

        void               drawVideoInfos();
        void               drawTitle();
        void               drawMenu();
        void               drawScene();
        void               drawDisplay();
        void               drawMessages();

        void               updateScene();
        void               updateMessages();

        bool               loadLevel();
};
