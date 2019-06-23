#pragma once

#include <string>
#include <memory>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glext.h>

#include <mesh.hpp>
#include <shader.hpp>
#include <texture.hpp>

using namespace std;

class Model
{
    public:
        Model(shared_ptr<Mesh> m, shared_ptr<Texture> t);
        ~Model();

        operator GLuint() const { return list; }

    protected:
        shared_ptr<Mesh>    mesh;
        shared_ptr<Shader>  shader;
        shared_ptr<Texture> texture;

        GLuint              list = -1;
};
