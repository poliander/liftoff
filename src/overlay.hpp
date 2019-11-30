#pragma once

#include "framebuffer.hpp"
#include "player.hpp"
#include "state.hpp"

using namespace std;

class Overlay
{
    public:
        Overlay(State& s, shared_ptr<Player> p);
        ~Overlay();

        void draw();

    private:
        State&                  state;

        unique_ptr<View>        view;
        shared_ptr<Player>      player;

        void                    drawVideoInfos();
        void                    drawMenu();
        void                    drawScene();
        void                    drawDisplay();
        void                    drawMessages();
};
