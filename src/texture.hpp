#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <stdio.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL_image.h>

class Texture
{
    public:
        Texture(const char *filename, bool mipmapping);
        ~Texture();

        operator GLuint() const { return resource; }

    protected:
        GLuint resource = -1;
};

#endif
