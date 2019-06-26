#pragma once

struct Configuration
{
    short           vid_width;
    short           vid_height;
    unsigned short  vid_aspect;
    unsigned short  vid_fullscreen;
    unsigned short  vid_vsync;

    short           aud_sfx;
    short           aud_music;
    int             aud_mixfreq;
};
