#ifndef ENTITY_HPP_
#define ENTITY_HPP_

using namespace std;

#include <math.h>
#include <memory>

#include <GL/gl.h>

#include "definitions.hpp"
#include "functions.hpp"
#include "state.hpp"

class Entity
{
    public:
        Entity();
        virtual ~Entity();

        void           setType(unsigned short t);

        bool           isCollider();
        bool           isFocusable();
        bool           isIdle();
        bool           isGone();
        bool           isAlive();
        bool           isColliding(State &s, shared_ptr<Entity> e);

        void           setPos(float x, float y, float z);
        void           setPosX(float x);
        void           setPosY(float y);
        void           setPosZ(float z);
        float          getPosX();
        float          getPosY();
        float          getPosZ();

        void           setVelocity(float x, float y, float z);
        float          getVelocityX();
        float          getVelocityY();
        float          getVelocityZ();

        void           setScale(float x, float y, float z);
        float          getScale();
        float          getScaleX();
        float          getScaleY();
        float          getScaleZ();

        void           setAcceleration(float x, float y, float z);
        void           setAccelerationX(float x);
        void           setAccelerationY(float y);
        void           setAccelerationZ(float z);

        void           setRotation(float x, float y, float z);
        float          getRotationX();
        float          getRotationY();
        float          getRotationZ();

        void           setSpin(float x, float y, float z);

        int            getLife();
        void           setLife(int l);

        void           activate();
        virtual void   damage(State &s, int p);
        virtual void   collide(State &s, shared_ptr<Entity> e);

        virtual void   move(State &s) = 0;
        virtual void   draw(State &s) = 0;

        static bool    sort(const shared_ptr<Entity> &e1, const shared_ptr<Entity> &e2);

    protected:
        unsigned short e_id;
        unsigned short e_type;
        unsigned short e_state;
 
        float          p_x;                  // position
        float          p_y;
        float          p_z;

        float          a_x = 0;              // acceleration
        float          a_y = 0;
        float          a_z = 0;

        float          v_x = 0;              // velocity
        float          v_y = 0;
        float          v_z = 0;

        float          s_x = 1.0f;           // scale
        float          s_y = 1.0f;
        float          s_z = 1.0f;

        float          r_x = 0;              // rotation
        float          r_y = 0;
        float          r_z = 0;

        float          w_x = 0;              // spin
        float          w_y = 0;
        float          w_z = 0;

        float          c_r = 1.0f;           // color
        float          c_g = 1.0f;
        float          c_b = 1.0f;
        float          c_a = 1.0f;

        int            life;
        bool           focusable = false;
};

#endif
