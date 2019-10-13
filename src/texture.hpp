#pragma once

#include <stdio.h>
#include <cstring>
#include <string>
#include <memory>

#include <GL/glew.h>
#include <GL/glext.h>

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

class Texture
{
    public:
        Texture(string filename);
        ~Texture();

        void bind();
        void draw();

    protected:
        GLuint vertexArrayObject;
        GLuint vertexArrayBuffers[2];
        GLuint texColorBuffer;

        void init();
        bool load(string filename, t_image *image);
};
