#pragma once

#include <memory>

#include "framebuffer.hpp"
#include "player.hpp"
#include "state.hpp"

using std::make_pair;
using std::unique_ptr;

class Overlay
{
    public:
        Overlay(State& s);
        ~Overlay();

        void update();
        void draw();

    private:
        State&                  state;

        unique_ptr<View>        view;
        unique_ptr<View>        perspective;
        unique_ptr<Framebuffer> framebuffer;

        void                    drawScreen();
        void                    drawMenu();
        void                    drawStatus();
        void                    drawMessages();

        float                   screen_y;
        float                   ship_y;

        float                   logo1_x;
        float                   logo1_y;
        float                   logo2_x;
        float                   logo2_y;

        bool                    menu_visible;
        bool                    menu_cursor_visible;
        float                   menu_alpha;

        bool                    status_visible;
        float                   status_alpha;
};
