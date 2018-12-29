#include "powerup.hpp"

Powerup::Powerup(State* sptr) {
	state = sptr;

	particles = new ParticleEngine(state);
	particles->setup(EMITTER_EXPLOSION, 20, .4f, .4f, .4f, 25.0f, 5.0f);
}

Powerup::~Powerup() {
}

void Powerup::move(int oid) {
	particles->move();
}

void Powerup::draw(int oid) {
	float a = 1.0f;

	if (state->objects[oid].pos_z < -8000.0f) {
		a = (state->objects[oid].pos_z + 10000.0f) * .0005f;
	}

	a *= float(state->global_alpha) * .01f;

	glLoadIdentity();
	glPushMatrix();

	glRotatef(-state->tilt_x*.035f, 0, 1, 0);
	glRotatef(-state->tilt_y*.035f, 1, 0, 0);

	glTranslatef(
		(state->objects[oid].pos_x - state->cam_x) * E_RELATIVE_MOVEMENT,
		(state->objects[oid].pos_y - state->cam_y) * E_RELATIVE_MOVEMENT,
		state->objects[oid].pos_z
	);

	glShadeModel(GL_FLAT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, state->texture[T_EXPLOSION_3]);

	a = 1.25f - state->objects[oid].cnt2 * .03f;
	if (a > .0f) {
		particles->setAlpha(a);
		particles->setSize(3.0f * 1.0f / (1.0f + state->objects[oid].cnt2 * .25f));
		particles->setColor(.6f, .75f, 1.0f);
		particles->setScale(25.0f + state->objects[oid].cnt2 * 5.0f);
		particles->draw(
				.0f, .0f, .0f,
				state->objects[oid].rot_x, state->objects[oid].rot_y, state->objects[oid].rot_z
		);
	} else {
		state->remove(oid);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);

	glPopMatrix();
}
