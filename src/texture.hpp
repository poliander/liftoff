#pragma once

#include <stdio.h>
#include <string>
#include <memory>

#include <GL/gl.h>
#include <GL/glext.h>

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

class Texture
{
    public:
        Texture(std::string filename, bool mipmap);
        ~Texture();

        operator GLuint() const { return resource; }

    protected:
        GLuint resource = -1;

        bool load(std::string filename, t_image *image);
        void bind(t_image *image, bool mipmap);
};
