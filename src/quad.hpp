#pragma once

#include <GL/glew.h>

class Quad
{
    public:
        Quad();
        virtual ~Quad();

    protected:
        GLuint vertexArray;
        GLuint vertexArrayBuffers[2];
};
