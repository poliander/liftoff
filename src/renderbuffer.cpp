#include "renderbuffer.hpp"

Renderbuffer::Renderbuffer(GLuint w, GLuint h, GLuint s)
{
    width = w;
    height = h;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &framebufferMSAA);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferMSAA);

    glGenRenderbuffers(1, &renderbufferColor);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferColor);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, s, GL_RGB8, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenRenderbuffers(1, &renderbufferDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferDepth);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, s, GL_DEPTH_COMPONENT, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbufferColor);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbufferDepth);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderbuffer::~Renderbuffer()
{
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteFramebuffers(1, &framebufferMSAA);
    glDeleteTextures(1, &texture);
}


void Renderbuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferMSAA);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
}

void Renderbuffer::blit()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferMSAA);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(
        0, 0, width, height,
        0, 0, width, height,
        GL_COLOR_BUFFER_BIT,
        GL_LINEAR
    );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
