#pragma once

#include "quad.hpp"

using namespace std;

class Framebuffer : public Quad
{
    public:
        Framebuffer();
        ~Framebuffer();

        void   bind();
        void   unbind();
        void   draw();

    protected:
        GLuint texColorBuffer;
        GLuint frameBuffer;
        GLint  viewport[4];
};
