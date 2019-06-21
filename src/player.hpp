#pragma once

#include <cmath>

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
        void            collide(State &s, shared_ptr<Entity> e);
        void            collect(unsigned short e_obj);

        void            setEnergy(int e);
        int             getEnergy();
        int             getEnergyMaximum();
        int             getEnergyRegeneration();

        int             getLifeMaximum();
        int             getLifeRegeneration();
        int             getLifeRegenerationEnergy();

    private:
        ParticleEngine* particles;

        unsigned short  powerup;
        unsigned short  powerup_booster_length;
        int             powerup_booster_timer;
        int             powerup_booster_ltimer;

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
