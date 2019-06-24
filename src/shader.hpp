#pragma once

#include <fstream>
#include <string>

#include <GL/glew.h>

#include "transform.hpp"

using namespace std;

class Shader
{
    public:
        Shader(string vsFilename, string fsFilename);
        ~Shader();

        void bind();
        void update(const Transform& transform, const Camera& camera);

    private:
        GLuint program;
        GLuint shaders[2];
        GLuint uniforms[3];

        string load(const string& filename);
        GLuint create(const string& text, unsigned int type);
};
