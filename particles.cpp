#include "particles.hpp"

ParticleEngine::ParticleEngine(State* sptr) {
	state = sptr;

	pemitter = EMITTER_JET;
	pr = 1.0f;
	pg = 1.0f;
	pb = 1.0f;
	palpha = 1.0f;
	pdrift = .0f;
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

	if (state->vid_cfg_lowquality) {
		pnum /= 2;
	}

	pdec = decay;
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
				p[i].fading = float(rand() % 100) / 1000.0f + 0.003f;

				p[i].dx = (float(rand() % 50) - 25.0f) * 10.0f;
				p[i].dy = (float(rand() % 50) - 25.0f) * 10.0f;
				p[i].dz = (float(rand() % 50) - 25.0f) * 10.0f;

				p[i].px = .0f;
				p[i].py = .0f;
				p[i].pz = .0f;
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
//	pdx = dx;
//	pdy = dy;
//	pdz = dz;
}

void ParticleEngine::setSize(float size) {
//	psize = size;
}

void ParticleEngine::setDrift(float drift) {
//	pdrift = drift;
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
			if (p[i].active) {
				p[i].lifetime -= state->timer_adjustment * p[i].fading;

				if (p[i].lifetime < .0f) {
					p[i].active = false;
				} else {
					p[i].px += p[i].dx * state->timer_adjustment;
					p[i].py += p[i].dy * state->timer_adjustment;
					p[i].pz += p[i].dz * state->timer_adjustment;

					p[i].dx += pdx * state->timer_adjustment;
					p[i].dy += pdy * state->timer_adjustment;
					p[i].dz += pdz * state->timer_adjustment;
				}
			}
		}
	} else {
		for (i=0; i<pnum_max; i++) {
			p[i].lifetime -= state->timer_adjustment * p[i].fading;
			p[i].pz += (p[i].dz * (.75f + .25f * p[i].fading)) * state->timer_adjustment;

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
	float sf = palpha * state->global_alpha * .005f;
	float size = psize * .5f;

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
	glScalef(15.0f, 15.0f, 1.0f);
	for (i=0; i<pnum; i++) {
		if (p[i].active) {
			glColor4f(pr * sf, pg * sf, pb * sf, p[i].lifetime * sf);

			glBegin(GL_QUADS);
			  glTexCoord2d(0, 0);
			  glVertex3f(p[i].px - size, p[i].py - size, p[i].pz);

			  glTexCoord2d(1, 0);
			  glVertex3f(p[i].px + size, p[i].py - size, p[i].pz);

			  glTexCoord2d(1, 1);
			  glVertex3f(p[i].px + size, p[i].py + size, p[i].pz);

			  glTexCoord2d(0, 1);
			  glVertex3f(p[i].px - size, p[i].py + size, p[i].pz);
			glEnd();
		}
	}

	glPopMatrix();
}
