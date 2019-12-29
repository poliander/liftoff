/*
 *  Copyright (C) 2019 René Pollesch <poliander@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

class Texture : public Quad {
    public:
        explicit Texture(string filename);
        ~Texture();

        void bind();

    protected:
        GLuint texColorBuffer;

        bool load(string filename, t_image* image);
};
