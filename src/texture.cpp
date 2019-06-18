#include "texture.hpp"

Texture::Texture(const char *filename, bool mipmap)
{
    SDL_Surface *target;
    SDL_RWops *rwop = SDL_RWFromFile(filename, "rb");
    SDL_Surface *image = IMG_LoadTGA_RW(rwop);

    target = SDL_CreateRGBSurface(SDL_SWSURFACE, image->w, image->h, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
            0x000000FF,
            0x0000FF00,
            0x00FF0000,
            0xFF000000
#else
            0xFF000000,
            0x00FF0000,
            0x0000FF00,
            0x000000FF
#endif
    );

    SDL_SetAlpha(image, 0, 0);
    SDL_BlitSurface(image, 0, target, 0);

    glGenTextures(1, &resource);
    glBindTexture(GL_TEXTURE_2D, resource);

    if (mipmap) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, target->pixels);

    SDL_FreeSurface(image);
    SDL_FreeSurface(target);
}

Texture::~Texture()
{
}
