#pragma once

#include <fstream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

enum UNIFORMS
{
    UNI_MVP,
    UNI_COLOR,
    UNI_TEXT_COLOR
};

class Shader
{
    public:
        Shader(const string vsFilename, const string fsFilename);
        ~Shader();

        void   bind();
        void   unbind();
        void   update(unsigned short u, glm::mat4 m);
        void   update(unsigned short u, glm::vec2 v);
        void   update(unsigned short u, glm::vec4 v);

    private:
        GLuint program;
        GLuint shaders[2];
        GLuint uniforms[3];

        string load(const string& filename);
        GLuint create(const string& text, unsigned int type);
};
