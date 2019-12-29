#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <stdlib.h>

#include "functions.hpp"
#include "framebuffer.hpp"
#include "state.hpp"

#define SKYBOX_NUM_STARS      3000
#define SKYBOX_NUM_STARS_WARP 500

using std::max;

class Skybox {
    public:
        explicit Skybox(State* s);
        ~Skybox();

        void                    update();
        void                    draw();

    private:
        float                   stars[SKYBOX_NUM_STARS][4];

        State*                  state;

        unique_ptr<View>        view;
        unique_ptr<Framebuffer> framebuffer;
};
