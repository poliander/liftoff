#ifndef CARGO_HPP_
#define CARGO_HPP_

#include <math.h>

#include "object.hpp"

class Cargo : public Object {
	public:
		Cargo(State* sptr);
		~Cargo();

		void draw(int oid);
};

extern Cargo* cargo;

#endif
