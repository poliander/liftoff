#pragma once

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>

#include "definitions.hpp"

struct config_t {
    uint16_t        vid_width;
    uint16_t        vid_height;

    uint8_t         vid_quality;
    uint8_t         vid_fullscreen;
    uint8_t         vid_vsync;

    uint8_t         aud_sfx;
    uint8_t         aud_music;
    uint16_t        aud_mixfreq;
};

class Configuration {
    public:
        static bool load(const char* dir, config_t* c);
        static bool save(const char* dir, config_t* c);
};
