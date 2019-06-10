#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <math.h>

#include "object.hpp"
#include "particles.hpp"

class Player : public Object {
    public:
        Player(State &s);
        ~Player();

        void            setTarget(int oid);
        int             getTarget(int oid);

        bool            shoot();
        void            tilt(float t);

        void            move();
        void            draw();

        int             acceleration;

        int             energy_max;
        int             energy_reg;

        int             shield_max;
        int             shield_reg;
        int             shield_reg_energy;
        
        unsigned short  gun_power;


    protected:
        ParticleEngine* particles;

        float           j_l;
        float           jt_l;
        float           jr;

        // gun fire flashes
        float           gun_flash[2];
        float           gun_flash_rot[2];

        unsigned short  powerup_booster_length;
        int             powerup_booster_timer;
        int             powerup_booster_ltimer;
};

extern Player* player;

#endif
