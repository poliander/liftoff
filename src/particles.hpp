#ifndef PARTICLES_HPP_
#define PARTICLES_HPP_

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "state.hpp"

#define EMITTER_JET       1
#define EMITTER_EXPLOSION 2

struct particle_t {
    bool active;

    float lifetime;
    float fading;

    float r;
    float g;
    float b;

    float px;
    float py;
    float pz;

    float dx;
    float dy;
    float dz;
};

class ParticleEngine {
    public:
        ParticleEngine(State* sptr);
        ~ParticleEngine();

        void            setup(short emitter, short particles, float dx, float dy, float dz, float decay, float size);

        void            setAlpha(float global_alpha);
        void            setColor(float r, float g, float b);
        void            setSize(float size);
        void            setScale(float scale);
        void            setParticleNumber(short particles);
        void            setDirection(float dx, float dy, float dz);
        void            setFadeFactor(float f);

        void            draw(float px, float py, float pz, float rx, float ry, float rz);
        void            move();

    protected:
        State*          state;
        particle_t      p[DEFAULT_GFX_PARTICLES];

        short           pemitter;
        short           pnum_max;
        short           pnum;

        float           pr, pg, pb;
        float           palpha;

        float           pscale;
        float           psize;
        float           pdx, pdy, pdz;

        void            moveJet();
        void            moveExplosion();
};

extern ParticleEngine* particles;

#endif
