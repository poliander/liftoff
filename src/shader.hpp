#pragma once

#include <string>

#include <GL/glew.h>

using namespace std;

class Shader
{
    public:
        Shader(string vsFilename, string fsFilename);
        ~Shader();

    private:
        GLuint resource;
};
