#pragma once

#include <stdio.h>
#include <cstring>
#include <string>
#include <memory>

#include "quad.hpp"

using namespace std;

#pragma pack(push, 1)

struct t_tga_header {
    char      id_length;
    char      colormap_type;
    char      data_type;
    short int colormap_origin;
    short int colormap_length;
    char      colormap_depth;
    short int x_origin;
    short int y_origin;
    short     width;
    short     height;
    char      bpp;
    char      image_descriptor;
};

#pragma pack(pop)

struct t_image {
    int       width, height;
    int       format;
    int       bpp;

    GLubyte*  data;
};

class Texture : public Quad
{
    public:
        Texture(string filename);
        ~Texture();

        void bind();

    protected:
        bool load(string filename, t_image *image);
};
