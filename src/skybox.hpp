#pragma once

#include <math.h>
#include <stdlib.h>

#include <state.hpp>

class Skybox
{
    public:
        Skybox();
        virtual ~Skybox();

        void move(State &s);
        void draw(State &s);

    private:
        float stars[8000][4];

        int num_stars = 3000;
        int num_stars_warp = 1000;
};
