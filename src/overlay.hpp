#pragma once

#include "framebuffer.hpp"
#include "state.hpp"

using namespace std;

class Overlay
{
    public:
        Overlay(State& s);
        ~Overlay();

        void draw();

    private:
        State&                  state;

        unique_ptr<View>        view;
        unique_ptr<Framebuffer> framebuffer;
};
