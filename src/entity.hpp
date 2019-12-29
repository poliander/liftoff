/*
 *  Copyright (C) 2019 René Pollesch <poliander@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <GL/glew.h>

#include <algorithm>
#include <memory>

#include "definitions.hpp"
#include "functions.hpp"
#include "state.hpp"

using std::shared_ptr;

class Entity {
    public:
        explicit Entity(State* s);
        virtual ~Entity();

        void               setType(uint16_t t);

        bool               isCollectable();
        bool               isCollider();
        bool               isScenery();
        bool               isFocusable();
        bool               isIdle();
        bool               isFading();
        bool               isGone();
        bool               isAlive();
        bool               isPlayer();
        bool               isColliding(shared_ptr<Entity> e);

        void               setPos(float x, float y, float z);
        void               setPosX(float x);
        void               setPosY(float y);
        void               setPosZ(float z);

        glm::vec3          getPos();
        float              getPosX();
        float              getPosY();
        float              getPosZ();

        void               setRot(float x, float y, float z);
        void               setRotX(float z);
        void               setRotY(float z);
        void               setRotZ(float z);
        float              getRotX();
        float              getRotY();
        float              getRotZ();

        void               setScale(float x, float y, float z);
        float              getScale();
        float              getScaleX();
        float              getScaleY();
        float              getScaleZ();

        void               setVelocity(float x, float y, float z);
        void               setVelocityX(float x);
        void               setVelocityY(float y);
        void               setVelocityZ(float z);
        float              getVelocityX();
        float              getVelocityY();
        float              getVelocityZ();

        void               setAcceleration(float x, float y, float z);
        void               setAccelerationX(float x);
        void               setAccelerationY(float y);
        void               setAccelerationZ(float z);

        void               setSpin(float x, float y, float z);

        float              getAcceleration();
        void               setLife(int l);
        int                getLife();

        void               setEnergy(int e);
        int                getEnergy();
        int                getEnergyMaximum();
        int                getEnergyRegeneration();

        int                getLifeMaximum();
        int                getLifeRegeneration();
        int                getLifeRegenerationEnergy();

        void               setMoney(int32_t m);
        int32_t            getMoney();

        void               init();
        void               activate();
        virtual bool       damage(int p);
        virtual void       collect(uint16_t e_obj);
        virtual void       collide(shared_ptr<Entity> e);
        virtual void       shoot();

        virtual void       update();
        virtual void       draw();
        void               drawCrosshair(shared_ptr<Entity> e);

        void               resetTarget();
        void               checkTarget(shared_ptr<Entity> e);
        bool               hasTarget(shared_ptr<Entity> e);

        static bool        sort(const shared_ptr<Entity> &e1, const shared_ptr<Entity> &e2);

    protected:
        uint16_t           e_obj;
        uint16_t           e_type;
        uint16_t           e_state;

        State*             state;

        float              p_x;                  // position
        float              p_y;
        float              p_z;

        float              a_x = 0;              // acceleration
        float              a_y = 0;
        float              a_z = 0;

        float              v_x = 0;              // velocity
        float              v_y = 0;
        float              v_z = 0;

        float              s_x = 1.0f;           // scale
        float              s_y = 1.0f;
        float              s_z = 1.0f;

        float              r_x = 0;              // rotation
        float              r_y = 0;
        float              r_z = 0;

        float              w_x = 0;              // spin
        float              w_y = 0;
        float              w_z = 0;

        float              c_r = 1.0f;           // color
        float              c_g = 1.0f;
        float              c_b = 1.0f;
        float              c_a = 1.0f;

        float              t_r = 1.0f;           // crosshair color
        float              t_g = .55f;
        float              t_b = .3f;

        shared_ptr<Entity> target;

        uint32_t           money;
        int                life;
        bool               focusable;
        float              acceleration;
        float              deceleration;
        uint16_t           gun_power;

        uint8_t            powerup;

        int                energy;
        int                energy_max;
        int                energy_reg;

        int                life_max;
        int                life_reg;
        int                life_reg_energy;

        float              calcDistance2D(shared_ptr<Entity> e);
        float              calcDistance3D(shared_ptr<Entity> e);
        float              calcDistanceScale();
};
