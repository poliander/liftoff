#pragma once

#include <memory>

#include "state.hpp"
#include "framebuffer.hpp"

class Renderbuffer {
    public:
        Renderbuffer(State& s);
        ~Renderbuffer();

        void bind();
        void unbind();
        void clear();
        void blit();
        void draw();

    protected:
        State&                  state;

        unique_ptr<View>        view;
        unique_ptr<Framebuffer> framebuffer;

        GLuint                  renderbuffer;
        GLuint                  renderbufferColor;
        GLuint                  renderbufferDepth;
};
