#pragma once

#include <cmath>

#include <GL/glew.h>
#include <GL/glu.h>

#include "state.hpp"

enum emitters_e {
    EMIT_JET,
    EMIT_EXPLOSION
};

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

        void               setAlpha(float a);
        void               setColor(float r, float g, float b);
        void               setContinuous(bool c);

        void               setVolume(float v);
        void               setInflation(float i);
        float              getInflation();

        void               setSize(float s);
        void               setIncrease(float i);
        float              getIncrease();

        bool               isGone();

        void               setup(short e, short particles, float dx, float dy, float dz, float decay, float size);
        void               update(State &s);
        void               draw(State &s, float px, float py, float pz, float rx, float ry, float rz);

    private:
        vector<particle_t> particles;

        short              pemitter;    // emitter type
        bool               pcontinuous; // continuously respawn every particle

        float              pvolume;     // volume
        float              pinflation;  // volume increase over time

        float              psize;       // particle size
        float              pincrease;   // particle size increase over time

        float              c_r;
        float              c_g;
        float              c_b;
        float              c_a;
};
