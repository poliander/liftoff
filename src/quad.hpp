#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

class Quad
{
    public:
        Quad();
        virtual ~Quad();

        void setPosition(glm::vec4 p);
        void setTextureCoordinates(glm::vec4 t);
        void draw();

    protected:
        glm::vec4 uv;
        glm::vec4 pos;

        bool changed;
        void update();

        GLuint vertexArray;
        GLuint vertexArrayBuffer;
};
