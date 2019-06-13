#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <stdio.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL_image.h>

#include <mesh.hpp>

class Model
{
    public:
        Model(const char *txt, const char *obj);
        ~Model();

        GLuint      getList();
        bool        hasError();

    protected:
        GLuint      texture;
        GLuint      list;
        short       status = 0;

        obj_model_t mesh;

        bool        loadTexture(const char *filename);
        bool        loadObject(const char *filename);

        bool        objectLoadFirstPass(FILE *fp);
        bool        objectLoadSecondPass(FILE *fp);
        bool        objectAllocate();
        void        objectFree();
};

#endif
