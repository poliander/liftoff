#pragma once

#include "framebuffer.hpp"
#include "player.hpp"
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
        unique_ptr<View>        perspective;
        unique_ptr<Framebuffer> framebuffer;

        void                    drawVideoInfos();
        void                    drawMenu();
        void                    drawScene();
        void                    drawDisplay();
        void                    drawMessages();
};
