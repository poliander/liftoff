#include "texture.hpp"

Texture::Texture(string filename)
{
    t_image *image;

    image = (t_image *)malloc(sizeof(t_image));
    image->data = nullptr;

    if (load(filename, image)) {
        assign(image);
    }

    if (image->data != nullptr) {
        free(image->data);
    }

    free(image);
}

Texture::~Texture()
{
}

bool Texture::load(string filename, t_image *image)
{
    t_tga_header header;
    unsigned char raw[4], trans[4];
    int rle_count = 0, rle_repeat = 0, read_next = 1, pixels, r;
    GLubyte *buffer;
    FILE *fd = fopen(filename.c_str(), "rb");

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
    
    pixels = image->width * image->height;
    buffer = (GLubyte *)malloc((image->bpp / 8) * pixels * sizeof(GLubyte));

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
                buffer[i * 3 + 0] = raw[2];
                buffer[i * 3 + 1] = raw[1];
                buffer[i * 3 + 2] = raw[0];
                break;

            case GL_RGBA:
                buffer[i * 4 + 0] = raw[2];
                buffer[i * 4 + 1] = raw[1];
                buffer[i * 4 + 2] = raw[0];
                buffer[i * 4 + 3] = raw[3];
                break;
        }

        --rle_count;
    }

    fclose(fd);
    
    image->data = (GLubyte *)malloc((image->bpp / 8) * pixels * sizeof(GLubyte));
    r = image->width * image->bpp / 8;

    for (int i = 0; i < image->height; ++i) {
        const GLubyte* srcBegin = buffer + (r * (image->height - i - 1));
        const GLubyte* srcEnd   = srcBegin + r;

        copy(srcBegin, srcEnd, image->data + r * i);
    }

    delete [] buffer;

    return true;
}

void Texture::assign(t_image *image)
{
    glGenTextures(1, &resource);
    glBindTexture(GL_TEXTURE_2D, resource);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, image->format, image->width, image->height, 0, image->format, GL_UNSIGNED_BYTE, image->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    float vertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(2, vertexArrayBuffers);

    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Texture::draw()
{
    glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, resource);
}
