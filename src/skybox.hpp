#pragma once

#include <cmath>
#include <stdlib.h>

#include <state.hpp>

#define SKYBOX_NUM_STARS      3000
#define SKYBOX_NUM_STARS_WARP 1000

class Skybox
{
    public:
        Skybox();
        ~Skybox();

        void move(State &s);
        void draw(State &s);

    private:
        float               stars[SKYBOX_NUM_STARS][4];
        shared_ptr<Texture> texture;
};
