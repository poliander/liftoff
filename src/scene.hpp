#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <utility>
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
#include "overlay.hpp"
#include "player.hpp"
#include "powerup.hpp"
#include "renderbuffer.hpp"
#include "skybox.hpp"

using std::unique_ptr;

class Scene {
    public:
        explicit Scene(State& s);
        ~Scene();

        void                load();
        void                update();
        void                draw(const unique_ptr<Renderbuffer> &b);

    private:
        State&              state;

        shared_ptr<Player>  player;
        unique_ptr<Skybox>  skybox;
        unique_ptr<Overlay> overlay;

        void                updateScene();
        bool                loadLevel();
};
