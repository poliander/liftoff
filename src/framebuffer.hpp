#pragma once

#include <GL/glew.h>

using namespace std;

class Framebuffer
{
    public:
        Framebuffer();
        ~Framebuffer();

        void bind();
        void unbind();
        void draw();

    protected:
        GLuint vertexArrayObject;
        GLuint vertexArrayBuffers[2];
        GLuint texColorBuffer;
        GLuint frameBuffer;
        GLint  viewport[4];

        void init();
};
