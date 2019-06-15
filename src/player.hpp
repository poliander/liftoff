#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <math.h>

#include "entity.hpp"
#include "particles.hpp"
#include "missile.hpp"

class Player : public Entity
{
    public:
        Player();
        ~Player();

        void            move(State &s);
        void            draw(State &s);

        void            shoot(State &s);
        void            collect(unsigned short item);

        unsigned short  getAcceleration();
        unsigned short  getMoney();

        void            setEnergy(int e);
        int             getEnergy();
        int             getEnergyMaximum();
        int             getEnergyRegeneration();

        int             getLifeMaximum();
        int             getLifeRegeneration();
        int             getLifeRegenerationEnergy();

        bool            isTarget(shared_ptr<Entity> &e);

    private:
        ParticleEngine* particles;

        unsigned short  powerup;
        unsigned short  powerup_booster_length;
        int             powerup_booster_timer;
        int             powerup_booster_ltimer;

        unsigned short  acceleration;
        unsigned short  money;

        int             energy;
        int             energy_max;
        int             energy_reg;

        int             life_max;
        int             life_reg;
        int             life_reg_energy;

        float           j_l;              // jet axhaust animation
        float           jt_l;
        float           jr;

        float           gun_flash[2];     // gun flashes
        float           gun_flash_rot[2];
        unsigned short  gun_power;
};

#endif
