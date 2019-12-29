#pragma once

#include <stdio.h>
#include <cstring>
#include <string>
#include <memory>

#include "quad.hpp"

using std::string;

#pragma pack(push, 1)

struct t_tga_header {
    char      id_length;
    char      colormap_type;
    char      data_type;
    int16_t   colormap_origin;
    int16_t   colormap_length;
    char      colormap_depth;
    int16_t   x_origin;
    int16_t   y_origin;
    int16_t   width;
    int16_t   height;
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
        GLuint texColorBuffer;

        bool load(string filename, t_image *image);
};
