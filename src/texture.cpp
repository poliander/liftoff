#include "texture.hpp"

Texture::Texture(const char *filename, bool mipmap)
{
    t_image *image = (t_image *)malloc(sizeof(t_image));

    if (load(filename, image)) {
        bind(image, mipmap);

        if (image->data) {
            free(image->data);
        }
    }

    free(image);
}

Texture::~Texture()
{
}

bool Texture::load(const char *filename, t_image *image)
{
    t_tga_header header;
    unsigned char raw[4], trans[4];
    int rle_count = 0, rle_repeat = 0, read_next = 1, p;

    FILE *fd = fopen(filename, "rb");

    if (!fd) {
        return false;
    }

    fread(&header, sizeof(header), 1, fd);

    image->width = header.width;
    image->height = header.height;
    image->bpp = header.bpp;

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
    
    p = image->width * image->height;
    image->data = (GLubyte *)malloc((image->bpp / 8) * p * sizeof(GLubyte));

    for (int i = 0; i < p; ++i) {
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

void Texture::bind(t_image *image, bool mipmap)
{
    int r = image->width * image->bpp / 8;
    GLubyte* data = new GLubyte[image->height * r];

    for (int i = 0; i < image->height; ++i) {
        const GLubyte* srcBegin = image->data + (r * (image->height - i - 1));
        const GLubyte* srcEnd   = srcBegin + r;

        std::copy(srcBegin, srcEnd, data + r * i);
    }

    glGenTextures(1, &resource);
    glBindTexture(GL_TEXTURE_2D, resource);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    if (mipmap) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, image->bpp / 8, image->width, image->height, 0, image->format, GL_UNSIGNED_BYTE, data);

    delete [] data;
}
