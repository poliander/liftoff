#ifndef EXPLOSION_HPP_
#define EXPLOSION_HPP_

#include "object.hpp"
#include "particles.hpp"

class Explosion : public Object {
    public:
        Explosion(State &s);
        ~Explosion();

        bool            add(unsigned short eid, float px, float py, float pz, float sp);
        void            move(int oid);
        void            draw(int oid);

    protected:
        ParticleEngine* particles;
};

extern Explosion* explosion;

#endif
