#include "framebuffer.hpp"

Framebuffer::Framebuffer(GLuint w, GLuint h) : Quad()
{
    width = w;
    height = h;

    // color buffer
    glGenTextures(1, &buffers[FB_BUFFER_COLOR]);
    glBindTexture(GL_TEXTURE_2D, buffers[FB_BUFFER_COLOR]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // depth buffer
    glGenRenderbuffers(1, &buffers[FB_BUFFER_DEPTH]);
    glBindRenderbuffer(GL_RENDERBUFFER, buffers[FB_BUFFER_DEPTH]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffers[FB_BUFFER_DEPTH]);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffers[FB_BUFFER_COLOR], 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
    glDeleteTextures(1, &buffers[FB_BUFFER_COLOR]);
    glDeleteRenderbuffers(1, &buffers[FB_BUFFER_DEPTH]);
    glDeleteFramebuffers(1, &framebuffer);
}

void Framebuffer::bind()
{
    glGetIntegerv(GL_VIEWPORT, originalViewport);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFramebuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, originalFramebuffer);
    glViewport(originalViewport[0], originalViewport[1], originalViewport[2], originalViewport[3]);
}

void Framebuffer::clear()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::draw()
{
    glBindTexture(GL_TEXTURE_2D, buffers[FB_BUFFER_COLOR]);
    Quad::draw();
    glBindTexture(GL_TEXTURE_2D, 0);
}
