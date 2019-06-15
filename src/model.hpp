#ifndef MODEL_HPP_
#define MODEL_HPP_

using namespace std;

#include <memory>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL_image.h>

#include <mesh.hpp>
#include <texture.hpp>

class Model
{
    public:
        Model(const char *txt, const char *obj);
        ~Model();

        operator GLuint() const { return list; }

    protected:
        obj_model_t         mesh;
        shared_ptr<Texture> texture;
        GLuint              list = -1;

        bool                load(const char *filename);
        bool                loadFirstPass(FILE *fp);
        bool                loadSecondPass(FILE *fp);

        bool                memAllocate();
        void                memFree();
};

#endif
