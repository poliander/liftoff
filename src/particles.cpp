#include "particles.hpp"

ParticleEngine::ParticleEngine(State &s) : state(s)
{
    c_r = 1.0f;
    c_g = 1.0f;
    c_b = 1.0f;
    c_a = 1.0f;

    pinflation = 0;
    pincrease = 0;

    blendSourceFactor = GL_SRC_ALPHA;
    blendDestFactor = GL_ONE;
}

ParticleEngine::~ParticleEngine()
{
}

void ParticleEngine::setup(uint16_t e, uint16_t n, float dx, float dy, float dz, float decay, float size)
{
    particles.clear();

    psize = size;
    pemitter = e;
    pcontinuous = false;
    pvolume = 1.0f;

    switch (pemitter) {
        case EMIT_JET:
            pcontinuous = true;
            c_a = .5f;

            for (int i = 0; i < n; i++) {
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
            for (int i = 0; i < n; i++) {
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

void ParticleEngine::setTexture(GLuint t)
{
    texture = t;
}

void ParticleEngine::setBlendFunc(GLenum s, GLenum d)
{
    blendSourceFactor = s;
    blendDestFactor = d;
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

bool ParticleEngine::done()
{
    return particles.size() == 0;
}

void ParticleEngine::update()
{
    pvolume = pvolume * (1.0f + (pinflation * state.global_timer));
    psize += pincrease * state.global_timer;

    if (psize < 0) {
        particles.clear();
        return;
    }

    if (pemitter == EMIT_EXPLOSION) {
        for (auto p = particles.begin(); p < particles.end(); ++p) {
            p->lifetime -= state.global_timer * p->fading;

            p->px += p->dx * pvolume * state.global_timer;
            p->py += p->dy * pvolume * state.global_timer;
            p->pz += p->dz * pvolume * state.global_timer;

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
            p->lifetime -= state.global_timer * p->fading;

            p->px += (p->dx * (.75f + .25f * p->fading)) * state.global_timer;
            p->py += (p->dy * (.75f + .25f * p->fading)) * state.global_timer;
            p->pz += (p->dz * (.75f + .25f * p->fading)) * state.global_timer;

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

void ParticleEngine::draw(float px, float py, float pz, float rx, float ry, float rz)
{
    float m[16];
    float a = state.global_alpha;

    glDisable(GL_DEPTH_TEST);
    glBlendFunc(blendSourceFactor, blendDestFactor);

    state.shaders[S_TEXTURE]->bind();
    state.textures[texture]->bind();

    for (auto p = particles.begin(); p < particles.end(); ++p) {
        state.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(c_r * a, c_g * a, c_b * a, c_a * a * p->lifetime));
        state.shaders[S_TEXTURE]->update(UNI_MVP, state.view->transform(
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

        state.textures[texture]->draw();
    }

    state.shaders[S_TEXTURE]->unbind();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}
