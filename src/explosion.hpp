#ifndef EXPLOSION_HPP_
#define EXPLOSION_HPP_

#include "particles.hpp"
#include "state.hpp"

class Explosion {
    public:
        Explosion(State* sptr);
        ~Explosion();

        bool            add(unsigned short eid, float px, float py, float pz, float sp);
        void            move(int oid);
        void            draw(int oid);

    protected:
        State*          state;
        ParticleEngine* particles;
};

#endif
