#pragma once

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
        Model(std::string textureFilename, std::string objectFilename);
        ~Model();

        operator GLuint() const { return list; }

    protected:
        obj_model_t              mesh;
        std::shared_ptr<Texture> texture;
        GLuint                   list = -1;

        bool                     load(std::string filename);
        bool                     loadFirstPass(FILE *fp);
        bool                     loadSecondPass(FILE *fp);

        bool                     memAllocate();
        void                     memFree();
};
