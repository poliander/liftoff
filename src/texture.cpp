/*
 *  LIFT-OFF: BEYOND GLAXIUM
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

#include "texture.hpp"

Texture::Texture(string filename) : Quad() {
    t_image* image = (t_image*)malloc(sizeof(t_image));

    if (load(filename, image)) {
        glGenTextures(1, &texColorBuffer);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);

        glTexImage2D(GL_TEXTURE_2D, 0, image->format, image->width, image->height, 0, image->format, GL_UNSIGNED_BYTE, image->data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        free(image->data);
    }

    free(image);
}

Texture::~Texture() {
    glDeleteTextures(1, &texColorBuffer);
}

bool Texture::load(string filename, t_image *image) {
    t_tga_header header;
    unsigned char raw[4], trans[4];
    int rle_count = 0, rle_repeat = 0, read_next = 1, pixels, r;
    FILE *fd = fopen(filename.c_str(), "rb");

    if (!fd) {
        return false;
    }

    fread(&header, sizeof(header), 1, fd);

    pixels = header.width * header.height;

    image->width = header.width;
    image->height = header.height;
    image->bpp = header.bpp;
    image->data = (GLubyte *)malloc((image->bpp / 8) * pixels * sizeof(GLubyte));

    switch (header.bpp) {
        case 24:
            image->format = GL_RGB;
            break;

        case 32:
            image->format = GL_RGBA;
            break;

        default:
            fclose(fd);
            return false;
    }

    for (int i = 0; i < pixels; ++i) {
        if (header.data_type == 10) {
            if (rle_count == 0) {
                int rle_cmd = fgetc(fd);

                rle_count = 1 + (rle_cmd & 127);
                rle_repeat = rle_cmd >> 7;

                read_next = 1;
            } else if (!rle_repeat) {
                read_next = 1;
            }
        } else {
            read_next = 1;
        }

        if (read_next) {
            for (int j = 0; j * 8 < image->bpp; ++j) {
                raw[j] = fgetc(fd);
            }

            read_next = 0;
        }

        switch (image->format) {
            case GL_RGB:
                image->data[i * 3 + 0] = raw[2];
                image->data[i * 3 + 1] = raw[1];
                image->data[i * 3 + 2] = raw[0];
                break;

            case GL_RGBA:
                image->data[i * 4 + 0] = raw[2];
                image->data[i * 4 + 1] = raw[1];
                image->data[i * 4 + 2] = raw[0];
                image->data[i * 4 + 3] = raw[3];
                break;
        }

        --rle_count;
    }

    fclose(fd);

    return true;
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
}
