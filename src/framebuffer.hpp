#pragma once

#include "quad.hpp"

enum E_BUFFERS
{
    FB_BUFFER_COLOR,
    FB_BUFFER_DEPTH,

    FB_MSAA_BUFFER_COLOR,
    FB_MSAA_BUFFER_DEPTH
};

class Framebuffer : public Quad
{
    public:
        Framebuffer(GLuint w, GLuint h, GLuint s);
        ~Framebuffer();

        void   bind();
        void   unbind();
        void   clear();
        void   draw();

    protected:
        GLuint width;
        GLuint height;
        GLuint samples;

        GLint  originalViewport[4];
        GLint  originalFramebuffer;

        GLuint buffers[4];
        GLuint framebuffer;
        GLuint framebufferMSAA;
};
