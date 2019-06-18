#pragma once

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "state.hpp"

#define EMITTER_JET       1
#define EMITTER_EXPLOSION 2

struct particle_t
{
    float lifetime;
    float fading;

    float px;
    float py;
    float pz;

    float dx;
    float dy;
    float dz;
};

class ParticleEngine
{
    public:
        ParticleEngine();
        ~ParticleEngine();

        void            setup(short emitter, short particles, float dx, float dy, float dz, float decay, float size);

        void            setAlpha(float global_alpha);
        void            setColor(float r, float g, float b);
        void            setSize(float size);
        void            setScale(float scale);
        void            setContinuous(bool c);

        void            draw(State &s, float px, float py, float pz, float rx, float ry, float rz);
        void            move(State &s);

    private:
        particle_t      p[DEFAULT_GFX_PARTICLES];

        bool            continuous;
        short           pemitter;
        short           pnum_max;
        short           pnum;

        float           c_r, c_g, c_b, c_a;

        float           pscale;
        float           psize;
        float           pdecay;
        float           pdx, pdy, pdz;
};
