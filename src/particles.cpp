#include "particles.hpp"

ParticleEngine::ParticleEngine()
{
    c_r = 1.0f;
    c_g = 1.0f;
    c_b = 1.0f;
    c_a = 1.0f;

    pinflation = 0;
    pincrease = 0;
}

ParticleEngine::~ParticleEngine()
{
}

void ParticleEngine::setup(short emitter, short num, float dx, float dy, float dz, float decay, float size)
{
    particles.clear();

    psize = size;
    pemitter = emitter;
    pcontinuous = false;
    pvolume = 1.0f;

    switch (pemitter) {
        case EMIT_JET:
            pcontinuous = true;
            c_a = .5f;

            for (int i = 0; i < num; i++) {
                particles.push_back({
                    1.0f,
                    float(10 + rand() % 10) / 80.0f,

                    0,
                    0,
                    float(rand() % 1000) * .001f,

                    dx,
                    dy,
                    dz
                });
            }
            break;

        case EMIT_EXPLOSION:
            for (int i = 0; i < num; i++) {
                float pdx = -dx * .5f + float(rand() % int(dx * 100)) * .01f;
                float pdy = -dy * .5f + float(rand() % int(dy * 100)) * .01f;
                float pdz = -dz * .5f + float(rand() % int(dz * 100)) * .01f;

                if ((pdx < -dx) || (pdx > dx)) pdx *= .5f;
                if ((pdy < -dy) || (pdy > dy)) pdy *= .5f;
                if ((pdz < -dz) || (pdz > dz)) pdz *= .5f;

                if ((pdx > -.05f) && (pdx < .05f)) pdx += -.05f + float(rand() % 20) * .01f;
                if ((pdy > -.05f) && (pdy < .05f)) pdy += -.05f + float(rand() % 20) * .01f;
                if ((pdz > -.05f) && (pdz < .05f)) pdz += -.05f + float(rand() % 20) * .01f;

                particles.push_back({
                    1.0f,
                    decay * (float(50 + rand() % 100) * .01f),

                    -psize * .5f + float(rand() % int(psize * 100)) * .01f,
                    -psize * .5f + float(rand() % int(psize * 100)) * .01f,
                    -psize * .5f + float(rand() % int(psize * 100)) * .01f,

                    pdx,
                    pdy,
                    pdz
                });
            }
            break;
    }
}

bool ParticleEngine::isGone()
{
    return particles.size() == 0;
}

void ParticleEngine::setColor(float r, float g, float b)
{
    c_r = r;
    c_g = g;
    c_b = b;
}

void ParticleEngine::setAlpha(float a)
{
    c_a = a;
}

void ParticleEngine::setSize(float s)
{
    psize = s;
}

void ParticleEngine::setVolume(float v)
{
    pvolume = v;
}

void ParticleEngine::setInflation(float i)
{
    pinflation = i;
}

float ParticleEngine::getInflation()
{
    return pinflation;
}

void ParticleEngine::setIncrease(float i)
{
    pincrease = i;
}

float ParticleEngine::getIncrease()
{
    return pincrease;
}

void ParticleEngine::setContinuous(bool c)
{
    pcontinuous = c;
}

void ParticleEngine::update(State &s)
{
    pvolume  = pvolume * (1.0f + (pinflation * s.timer_adjustment));
    psize   += pincrease * s.timer_adjustment;

    if (psize < 0) {
        particles.clear();
        return;
    }

    if (pemitter == EMIT_EXPLOSION) {
        for (auto p = particles.begin(); p < particles.end(); ++p) {
            p->lifetime -= s.timer_adjustment * p->fading;

            p->px += p->dx * pvolume * s.timer_adjustment;
            p->py += p->dy * pvolume * s.timer_adjustment;
            p->pz += p->dz * pvolume * s.timer_adjustment;

            if (p->lifetime < 0) {
                if (pcontinuous) {
                    p->lifetime += 1.0f;

                    p->px = 0;
                    p->py = 0;
                    p->pz = 0;
                } else {
                    p = particles.erase(p);
                }
            }
        }
    } else {
        for (auto p = particles.begin(); p < particles.end(); ++p) {
            p->lifetime -= s.timer_adjustment * p->fading;

            p->px += (p->dx * (.75f + .25f * p->fading)) * s.timer_adjustment;
            p->py += (p->dy * (.75f + .25f * p->fading)) * s.timer_adjustment;
            p->pz += (p->dz * (.75f + .25f * p->fading)) * s.timer_adjustment;

            if (p->lifetime < 0) {
                if (pcontinuous) {
                    p->lifetime += 1.0f;

                    p->px = 0;
                    p->py = 0;
                    p->pz = 0;
                } else {
                    p = particles.erase(p);
                }
            }
        }
    }
}

void ParticleEngine::draw(State &s, float px, float py, float pz, float rx, float ry, float rz)
{
    float m[16];
    float a = s.global_alpha * .01f;

    glDisable(GL_DEPTH_TEST);

    s.shaders[S_TEXTURE]->bind();

    for (auto p = particles.begin(); p < particles.end(); ++p) {
        s.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(c_r * a, c_g * a, c_b * a, c_a * a * p->lifetime));
        s.shaders[S_TEXTURE]->update(UNI_MVP, s.view->transform(
            px + p->px,
            py + p->py,
            pz + p->pz,

            0,
            0,
            0,

            psize,
            psize,
            0
        ));

        s.textures[T_STAR]->draw();
    }

    s.shaders[S_TEXTURE]->unbind();

    glEnable(GL_DEPTH_TEST);
}
