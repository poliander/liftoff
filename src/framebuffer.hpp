#pragma once

#include "quad.hpp"

class Framebuffer : public Quad
{
    public:
        Framebuffer(GLuint w, GLuint h, GLuint f);
        ~Framebuffer();

        void   bind();
        void   unbind();
        void   draw();

    protected:
        GLuint width;
        GLuint height;

        GLint  originalViewport[4];
        GLint  originalFramebuffer;

        GLuint texture;
        GLuint framebuffer;
};
