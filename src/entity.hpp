#pragma once

#include <memory>

#include <GL/glew.h>

#include "definitions.hpp"
#include "functions.hpp"
#include "state.hpp"

using namespace std;

class Entity
{
    public:
        Entity();
        virtual ~Entity();

        void               setType(unsigned short t);

        bool               isCollectable();
        bool               isCollider();
        bool               isFocusable();
        bool               isIdle();
        bool               isFading();
        bool               isGone();
        bool               isAlive();
        bool               isPlayer();
        bool               isColliding(State &s, shared_ptr<Entity> e);

        void               setPos(float x, float y, float z);
        void               setPosX(float x);
        void               setPosY(float y);
        void               setPosZ(float z);
        float              getPosX();
        float              getPosY();
        float              getPosZ();

        float              getAcceleration();

        void               setVelocity(float x, float y, float z);
        void               setVelocityX(float x);
        void               setVelocityY(float y);
        void               setVelocityZ(float z);
        float              getVelocityX();
        float              getVelocityY();
        float              getVelocityZ();

        void               setScale(float x, float y, float z);
        float              getScale();
        float              getScaleX();
        float              getScaleY();
        float              getScaleZ();

        void               setAcceleration(float x, float y, float z);
        void               setAccelerationX(float x);
        void               setAccelerationY(float y);
        void               setAccelerationZ(float z);

        void               setRot(float x, float y, float z);
        float              getRotX();
        float              getRotY();
        float              getRotZ();

        void               setSpin(float x, float y, float z);

        int                getLife();
        void               setLife(int l);

        void               addMoney(unsigned short m);
        void               setMoney(unsigned short m);
        unsigned short     getMoney();

        void               activate();
        virtual bool       damage(State &s, int p);
        virtual void       collect(unsigned short e_obj);
        virtual void       collide(State &s, shared_ptr<Entity> e);
        virtual void       shoot(State &s);

        virtual void       move(State &s);
        virtual void       draw(State &s) = 0;

        void               drawCrosshair(State &s, shared_ptr<Entity> e);

        void               resetTarget();
        void               checkTarget(State &s, shared_ptr<Entity> e);
        bool               hasTarget(shared_ptr<Entity> e);

        static bool        sort(const shared_ptr<Entity> &e1, const shared_ptr<Entity> &e2);

    protected:
        unsigned short     e_obj;
        unsigned short     e_type;
        unsigned short     e_state;
 
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

        unsigned short     money = 0;
        int                life = 1;
        bool               focusable = false;
        float              acceleration;

        float              calcDistance2D(State &s, shared_ptr<Entity> e);
        float              calcDistance3D(State &s, shared_ptr<Entity> e);
};
