#pragma once

#include <cmath>
#include <stdlib.h>

#include "functions.hpp"
#include "framebuffer.hpp"
#include "state.hpp"

#define SKYBOX_NUM_STARS      3000
#define SKYBOX_NUM_STARS_WARP 500

class Skybox
{
    public:
        Skybox(unsigned short fb_size);
        ~Skybox();

        void                    update(State &s);
        void                    draw(State &s);

    private:
        float                   stars[SKYBOX_NUM_STARS][4];
        shared_ptr<Framebuffer> framebuffer;
};
