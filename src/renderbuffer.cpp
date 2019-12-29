#include "renderbuffer.hpp"

Renderbuffer::Renderbuffer(State* s) : state(s) {
    view = View::createOrthographic(
        state->vid_width * -.5f,
        state->vid_width * .5f,
        state->vid_height * .5f,
        state->vid_height * -.5f
    );

    framebuffer = make_unique<Framebuffer>(state->vid_fb_size, state->vid_fb_size, state->vid_multisampling);

    glGenRenderbuffers(1, &renderbufferColor);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferColor);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, state->vid_multisampling, GL_RGBA8, state->vid_fb_size, state->vid_fb_size);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenRenderbuffers(1, &renderbufferDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferDepth);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, state->vid_multisampling, GL_DEPTH_COMPONENT, state->vid_fb_size, state->vid_fb_size);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &renderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, renderbuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbufferColor);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbufferDepth);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderbuffer::~Renderbuffer() {
    glDeleteFramebuffers(1, &renderbuffer);
    glDeleteRenderbuffers(1, &renderbufferDepth);
    glDeleteRenderbuffers(1, &renderbufferColor);
}

void Renderbuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, renderbuffer);
    glViewport(0, 0, state->vid_fb_size, state->vid_fb_size);
}

void Renderbuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, state->vid_width, state->vid_height);
}

void Renderbuffer::clear() {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderbuffer::blit() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, renderbuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *framebuffer);
    glBlitFramebuffer(
        0, 0, state->vid_fb_size, state->vid_fb_size,
        0, 0, state->vid_fb_size, state->vid_fb_size,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );
}

void Renderbuffer::draw() {
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    state->shaders[S_TEXTURE]->bind();
    state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    state->shaders[S_TEXTURE]->update(UNI_MVP, view->transform(
        0, 0, state->vid_width, state->vid_height
    ));

    framebuffer->draw();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}
