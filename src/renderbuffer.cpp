#include "renderbuffer.hpp"

Renderbuffer::Renderbuffer(GLuint w, GLuint h, GLuint s)
{
    width = w;
    height = h;

    glGenRenderbuffers(1, &renderbufferColor);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferColor);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, s, GL_RGB8, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenRenderbuffers(1, &renderbufferDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferDepth);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, s, GL_DEPTH_COMPONENT, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbufferColor);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbufferDepth);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderbuffer::~Renderbuffer()
{
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &renderbufferDepth);
    glDeleteRenderbuffers(1, &renderbufferColor);
}

void Renderbuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
}

void Renderbuffer::blit()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(
        0, 0, width, height,
        0, 0, width, height,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
