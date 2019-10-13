#pragma once

#include "quad.hpp"

using namespace std;

class Framebuffer : public Quad
{
    public:
        Framebuffer();
        ~Framebuffer();

        void bind();
        void unbind();

    protected:
        GLuint frameBuffer;
        GLint  viewport[4];
};
