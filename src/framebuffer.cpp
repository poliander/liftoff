#include "framebuffer.hpp"

Framebuffer::Framebuffer(GLint s, GLint f)
{
    size = s;
    format = f;

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);

    glTexImage2D(GL_TEXTURE_2D, 0, f, s, s, 0, f, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &frameBuffer);
}

void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, size, size);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void Framebuffer::draw()
{
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    Quad::draw();
    glBindTexture(GL_TEXTURE_2D, 0);
}
