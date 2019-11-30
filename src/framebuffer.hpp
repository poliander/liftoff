#pragma once

#include "quad.hpp"

using namespace std;

class Framebuffer : public Quad
{
    public:
        Framebuffer(GLint s, GLint f);
        ~Framebuffer();

        void   bind();
        void   unbind();
        void   draw();

    protected:
        GLint  viewport[4];
        GLint  size;
        GLint  format;

        GLuint texColorBuffer;
        GLuint frameBuffer;
};
