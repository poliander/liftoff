#pragma once

#include "quad.hpp"

enum E_BUFFERS
{
    FB_BUFFER_COLOR,
    FB_BUFFER_DEPTH,
};

class Framebuffer : public Quad
{
    public:
        Framebuffer(GLuint w, GLuint h);
        ~Framebuffer();

        void   bind();
        void   unbind();
        void   clear();
        void   draw();

    protected:
        GLuint width;
        GLuint height;

        GLint  originalViewport[4];
        GLint  originalFramebuffer;

        GLuint buffers[2];
        GLuint framebuffer;
};
