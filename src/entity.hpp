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

        bool           isCollider();
        bool           isFocusable();
        bool           isIdle();
        bool           isGone();
        bool           isColliding(shared_ptr<Entity> e);

        void           setPos(float x, float y, float z);
        void           setPosX(float x);
        void           setPosY(float y);
        void           setPosZ(float z);

        float          getPosX();
        float          getPosY();
        float          getPosZ();

        void           setScale(float x, float y, float z);
        float          getScaleX();
        float          getScaleY();
        float          getScaleZ();

        void           setSpin(float x, float y, float z);

        int            getLife();
        void           setLife(int l);

        void           activate();
        virtual void   damage(State &s, int p);
        virtual void   collide(State &s, shared_ptr<Entity> e);

        virtual void   move(State &s) = 0;
        virtual void   draw(State &s) = 0;

    protected:
        unsigned short e_id;
        unsigned short e_type;
        unsigned short e_state;

        float          p_x, p_y, p_z;        // position
        float          r_x, r_y, r_z;        // rotation
        float          s_x, s_y, s_z;        // scale

        float          v_x = 0;              // velocity
        float          v_y = 0;
        float          v_z = 0;

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
