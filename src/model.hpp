#pragma once

#include <string>
#include <memory>

#include <GL/glew.h>

#include <camera.hpp>
#include <object.hpp>
#include <shader.hpp>
#include <texture.hpp>
#include <transform.hpp>

using namespace std;

enum VerexArrayBufferIndex
{
    VAB_INDICES,
    VAB_POSITIONS,
    VAB_TEXCOORDS,
    VAB_NORMALS
};

class State;

class Model
{
    public:
        Model(shared_ptr<Object> o, shared_ptr<Texture> t, shared_ptr<Shader> s);
        ~Model();

        void draw(const Transform& transform, const Camera& camera);

    protected:
        shared_ptr<Object>  object;
        shared_ptr<Shader>  shader;
        shared_ptr<Texture> texture;

        GLuint vertexArrayObject;
        GLuint vertexArrayBuffers[4];
};
