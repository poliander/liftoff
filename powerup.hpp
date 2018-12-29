#ifndef POWERUP_HPP_
#define POWERUP_HPP_

#include "state.hpp"
#include "particles.hpp"

class Powerup {
	public:
		Powerup(State* sptr);
		~Powerup();

		GLuint textures[1];

		void move(int oid);
		void draw(int oid);

	private:
		State*			state;
		ParticleEngine*	particles;
};

#endif
