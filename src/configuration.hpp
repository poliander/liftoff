#pragma once

#include <fcntl.h>
#include <stdio.h>

#include "definitions.hpp"

struct config_t
{
    short           vid_width;
    short           vid_height;
    unsigned short  vid_quality;
    unsigned short  vid_fullscreen;
    unsigned short  vid_vsync;

    short           aud_sfx;
    short           aud_music;
    int             aud_mixfreq;
};

class Configuration
{
    public:
        static bool load(const char* dir, config_t* c);
        static bool save(const char* dir, config_t* c);
};
