#pragma once

#include "quad.hpp"

using namespace std;

class Framebuffer : public Quad
{
    public:
        Framebuffer(GLint w, GLint h, GLint f);
        ~Framebuffer();

        void   bind();
        void   unbind();
        void   draw();

    protected:
        GLint  viewport[4];

        GLint width;
        GLint height;
        GLint format;

        GLuint texColorBuffer;
        GLuint frameBuffer;
};
