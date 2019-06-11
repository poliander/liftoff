#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <GL/gl.h>

#include "definitions.hpp"

class State;

class Entity
{
    public:
        Entity();
        virtual ~Entity();

        bool           isCollider();
        bool           isFocusable();
        bool           isIdle();
        bool           isGone();

        void           setActive();

        void           setPos(float x, float y, float z);

        void           setPosX(float x);
        void           setPosY(float y);
        void           setPosZ(float z);

        float          getPosX();
        float          getPosY();
        float          getPosZ();

        virtual void   move(State &s) = 0;
        virtual void   draw(State &s) = 0;

    protected:
        unsigned short e_id;
        unsigned short e_type;
        unsigned short e_state;

        float          p_x, p_y, p_z;        // position
        float          v_x, v_y, v_z;        // velocity
        float          s_x, s_y, s_z;        // scale
        float          c_r, c_g, c_b, c_a;   // color

        bool           focusable = false;
};

#endif
