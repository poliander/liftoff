#pragma once

using namespace std;

#include <cstring>
#include <string>
#include <memory>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glext.h>

#include <mesh.hpp>
#include <texture.hpp>

class Model
{
    public:
        Model(string textureFilename, string objectFilename);
        ~Model();

        operator GLuint() const { return list; }

    protected:
        obj_model_t         mesh;
        shared_ptr<Texture> texture;
        GLuint              list = -1;

        bool                load(string filename);
        bool                loadFirstPass(FILE *fp);
        bool                loadSecondPass(FILE *fp);

        bool                memAllocate();
        void                memFree();
};
