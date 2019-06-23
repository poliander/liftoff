#pragma once

#include <fstream>
#include <string>

#include <GL/glew.h>

using namespace std;

class Shader
{
    public:
        Shader(string vsFilename, string fsFilename);
        ~Shader();

    private:
        GLuint program;
        GLuint shaders[2];

        string load(const string& filename);
        GLuint create(const string& text, unsigned int type);
};
