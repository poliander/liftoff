#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL_image.h>
#include <GL/gl.h>

#include "model.hpp"
#include "state.hpp"

class Object
{
    public:
        Object(State &s);
        ~Object();

        GLuint      textures[4];

        bool        load(const char* datadir, const char* filename);
        void        unload();

        void        move(int oid);
        void        draw(int oid);
        void        drawCrosshair(int oid, float r, float g, float b);

        void        setPos(int oid, float px, float py, float pz);
        void        addSpeed(int oid, float sx, float sy, float sz);

        void        accelerateX(int oid, float ax);
        void        accelerateY(int oid, float ay);
        void        accelerateZ(int oid, float az);

        void        setRot(int oid, float rx, float ry, float rz);
        float       getRotX(int oid);
        float       getRotY(int oid);
        float       getRotZ(int oid);

        void        setSpin(int oid, float sx, float sy, float sz);
        float       getSpinX(int oid);
        float       getSpinY(int oid);
        float       getSpinZ(int oid);

    protected:
        State&      state;

        obj_model_t model;
        GLuint      model_list;

        float       alpha;
        float       shininess;
        float       brightness;

        bool        _load_OBJ_firstPass(FILE *fp);
        bool        _load_OBJ_secondPass(FILE *fp);
        bool        _load_OBJ_malloc();
        void        _load_OBJ_freemem();
};

#endif
