#pragma once

#include <GL/glew.h>

class Quad
{
    public:
        Quad();
        virtual ~Quad();

        void draw();

    protected:
        GLuint vertexArrayObject;
        GLuint vertexArrayBuffers[2];
        GLuint texColorBuffer;
};
