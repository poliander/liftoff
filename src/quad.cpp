#include "quad.hpp"

Quad::Quad()
{
    GLfloat vertices[6][4] = {
        { -0.5f,  0.5f,   0.0f, 0.0f },
        { -0.5f, -0.5f,   0.0f, 1.0f },
        {  0.5f, -0.5f,   1.0f, 1.0f },

        { -0.5f,  0.5f,   0.0f, 0.0f },
        {  0.5f, -0.5f,   1.0f, 1.0f },
        {  0.5f,  0.5f,   1.0f, 0.0f }
    };

    glGenBuffers(1, &vertexArrayBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Quad::~Quad()
{
}
