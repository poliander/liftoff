#include "particles.hpp"

ParticleEngine::ParticleEngine(State &s) : state(s) {
    pemitter = EMITTER_JET;
    pr = 1.0f;
    pg = 1.0f;
    pb = 1.0f;
    palpha = 1.0f;
    pscale = 1.0f;
}

ParticleEngine::~ParticleEngine() {
}

void ParticleEngine::setup(short emitter, short particles, float dx, float dy, float dz, float decay, float size) {
    int i;

    if (particles > DEFAULT_GFX_PARTICLES) {
        pnum = DEFAULT_GFX_PARTICLES;
    } else {
        pnum = particles;
    }

    psize = size;
    pdx = dx;
    pdy = dy;
    pdz = dz;
    pemitter = emitter;
    pnum_max = pnum;

    switch(pemitter) {
        case EMITTER_JET:
            for (i=0; i<pnum; i++) {
                p[i].active = true;
                p[i].lifetime = 1.0f;
                p[i].fading = float(10 + rand() % 10) / 80.0f;

                p[i].dx = dx;
                p[i].dy = dy;
                p[i].dz = dz;

                p[i].px = .0f;
                p[i].py = .0f;
                p[i].pz = float(rand() % 1000) * .001f;
            }
            break;

        case EMITTER_EXPLOSION:
            for (i=0; i<pnum; i++) {
                p[i].active = true;
                p[i].lifetime = 1.0f;
                p[i].fading = decay * (float(rand() % 50) * .0001f + .002f);
                p[i].dx = -pdx*.5f + float(rand() % int(pdx*100.0f)) *.01f;
                p[i].dy = -pdy*.5f + float(rand() % int(pdy*100.0f)) *.01f;
                p[i].dz = -pdz*.5f + float(rand() % int(pdz*100.0f)) *.01f;
                if ((p[i].dx < -pdx) || (p[i].dx > pdx)) p[i].dx *= .5f;
                if ((p[i].dy < -pdy) || (p[i].dy > pdy)) p[i].dy *= .5f;
                if ((p[i].dz < -pdz) || (p[i].dz > pdz)) p[i].dz *= .5f;
                if ((p[i].dx > -.05f) && (p[i].dx < .05f)) p[i].dx += -.05f + float(rand() % 20)*.01f;
                if ((p[i].dy > -.05f) && (p[i].dy < .05f)) p[i].dy += -.05f + float(rand() % 20)*.01f;
                if ((p[i].dz > -.05f) && (p[i].dz < .05f)) p[i].dz += -.05f + float(rand() % 20)*.01f;
                p[i].px = -psize*.5f + float(rand() % int(psize*100)) * .01f;
                p[i].py = -psize*.5f + float(rand() % int(psize*100)) * .01f;
                p[i].pz = -psize*.5f + float(rand() % int(psize*100)) * .01f;
            }
            break;
    }
}

void ParticleEngine::setAlpha(float global_alpha) {
    palpha = global_alpha;
}

void ParticleEngine::setColor(float r, float g, float b) {
    pr = r;
    pg = g;
    pb = b;
}

void ParticleEngine::setDirection(float dx, float dy, float dz) {
    pdx = dx;
    pdy = dy;
    pdz = dz;
}

void ParticleEngine::setSize(float size) {
    psize = size;
}

void ParticleEngine::setScale(float scale) {
    pscale = scale;
}

void ParticleEngine::setParticleNumber(short particles) {
    if (particles > pnum_max) {
        pnum = pnum_max;
    } else {
        pnum = particles;
    }
}

void ParticleEngine::move() {
    int i;

    if (pemitter == EMITTER_EXPLOSION) {
        for (i=0; i<pnum_max; i++) {
            p[i].px += p[i].dx * p[i].lifetime * state.timer_adjustment;
            p[i].py += p[i].dy * p[i].lifetime * state.timer_adjustment;
            p[i].pz += p[i].dz * p[i].lifetime * state.timer_adjustment;

            p[i].lifetime -= p[i].fading * state.timer_adjustment;

            if (p[i].lifetime < 0) {
                p[i].px = 0;
                p[i].py = 0;
                p[i].pz = 0;
                p[i].lifetime += .01f * float(66 + rand() % 33);
                p[i].dx = -pdx*.5f + float(rand() % int(pdx*100.0f)) *.01f;
                p[i].dy = -pdy*.5f + float(rand() % int(pdy*100.0f)) *.01f;
                p[i].dz = -pdz*.5f + float(rand() % int(pdz*100.0f)) *.01f;
                if ((p[i].dx < -pdx) || (p[i].dx > pdx)) p[i].dx *= .5f;
                if ((p[i].dy < -pdy) || (p[i].dy > pdy)) p[i].dy *= .5f;
                if ((p[i].dx > -.05f) && (p[i].dx < .05f)) p[i].dx += -.05f + float(rand() % 10)*.001f;
                if ((p[i].dy > -.05f) && (p[i].dy < .05f)) p[i].dy += -.05f + float(rand() % 10)*.001f;
            }
        }
    } else {
        for (i=0; i<pnum_max; i++) {
            p[i].lifetime -= state.timer_adjustment * p[i].fading;
            p[i].pz += (p[i].dz * (.75f + .25f * p[i].fading)) * state.timer_adjustment;

            if (p[i].lifetime < .0f) {
                p[i].lifetime += 1.0f;
                p[i].fading = float(10 + rand() % 10) / 80.0f;

                p[i].px = .0f;
                p[i].py = .0f;
                p[i].pz = .0f;
            }
        }
    }
}

void ParticleEngine::draw(float px, float py, float pz, float rx, float ry, float rz) {
    int i;
    float m[16];
    float sf = palpha * state.global_alpha * .005f;

    glPushMatrix();

    // set up position and rotation
    glTranslatef(px, py, pz);
    glRotatef(rx, 1, 0, 0);
    glRotatef(ry, 0, 1, 0);
    glRotatef(rz, 0, 0, 1);

    // let the particles face the observer
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f;
    m[4] = 0.0f; m[5] = 1.0f; m[6] = 0.0f;
    glLoadMatrixf(m);

    // render particles
    for (i=0; i<pnum; i++) {
        if (p[i].active) {
            glColor4f(pr * sf, pg * sf, pb * sf, p[i].lifetime * sf);

            glBegin(GL_QUADS);
              glTexCoord2d(0, 0);
              glVertex3f(pscale * (p[i].px - psize), pscale * (p[i].py - psize), pscale * p[i].pz);

              glTexCoord2d(1, 0);
              glVertex3f(pscale * (p[i].px + psize), pscale * (p[i].py - psize), pscale * p[i].pz);

              glTexCoord2d(1, 1);
              glVertex3f(pscale * (p[i].px + psize), pscale * (p[i].py + psize), pscale * p[i].pz);

              glTexCoord2d(0, 1);
              glVertex3f(pscale * (p[i].px - psize), pscale * (p[i].py + psize), pscale * p[i].pz);
            glEnd();
        }
    }

    glPopMatrix();
}
