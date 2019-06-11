#ifndef POWERUP_HPP_
#define POWERUP_HPP_

#include "object.hpp"
#include "particles.hpp"

class Powerup : public Object
{
    public:
        Powerup(State &s);
        ~Powerup();

        void            move(int oid);
        void            draw(int oid);

    private:
        ParticleEngine* particles;
};

#endif
