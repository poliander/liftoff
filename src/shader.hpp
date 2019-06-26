#pragma once

#include <fstream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

class Shader
{
    public:
        Shader(string vsFilename, string fsFilename);
        ~Shader();

        void bind();
        void update(glm::mat4 mvp);

    private:
        GLuint program;
        GLuint shaders[2];
        GLuint uniforms[3];

        string load(const string& filename);
        GLuint create(const string& text, unsigned int type);
};
