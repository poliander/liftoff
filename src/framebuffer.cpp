#include "framebuffer.hpp"

Framebuffer::Framebuffer(GLint w, GLint h, GLint f)
{
    width = w;
    height = h;
    format = f;

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);

    glTexImage2D(GL_TEXTURE_2D, 0, f, w, h, 0, f, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
    glViewport(0, 0, width, height);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void Framebuffer::draw()
{
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glBindVertexArray(vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
