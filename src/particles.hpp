#pragma once

#include <cmath>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>

#include "state.hpp"

enum emitters_e {
    EMIT_JET,
    EMIT_EXPLOSION
};

struct particle_t {
    float lifetime;
    float fading;

    float px;
    float py;
    float pz;

    float dx;
    float dy;
    float dz;
};

class ParticleEngine {
    public:
        explicit ParticleEngine(State* s);
        ~ParticleEngine();

        void               setTexture(GLuint t);
        void               setBlendFunc(GLenum s, GLenum d);

        void               setColor(float r, float g, float b);
        void               setAlpha(float a);

        void               setContinuous(bool c);
        void               setVolume(float v);
        void               setInflation(float i);
        float              getInflation();

        void               setSize(float s);
        void               setIncrease(float i);
        float              getIncrease();

        void               setup(uint16_t e, uint16_t n, float dx, float dy, float dz, float decay, float size);
        bool               done();

        void               update();
        void               draw(float px, float py, float pz, float rx, float ry, float rz);

    private:
        State*             state;
        vector<particle_t> particles;

        GLuint             texture;
        GLenum             blendSourceFactor;
        GLenum             blendDestFactor;

        uint16_t           pemitter;    // emitter type
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
