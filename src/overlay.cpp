#include "overlay.hpp"

Overlay::Overlay(State& s) : state(s)
{
    view = View::createPerspective(65.0f, 1.0f, .01f, 10000.0f);
    framebuffer = make_unique<Framebuffer>(s.vid_framebuffer_size, GL_RGB);
}

Overlay::~Overlay()
{
}

void Overlay::draw()
{
}
