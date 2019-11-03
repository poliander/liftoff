#include "quad.hpp"

Quad::Quad()
{
    glGenBuffers(1, &vertexArrayBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 6, NULL, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    setPosition(glm::vec4(-0.5f, -0.5f, 0.5f, 0.5f));
    setTextureCoordinates(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    update();
}

Quad::~Quad()
{
}

void Quad::setPosition(glm::vec4 p)
{
    pos = p;
}

void Quad::setTextureCoordinates(glm::vec4 u)
{
    uv = u;
}

void Quad::update()
{
    GLfloat vertices[6][4] = {
        { pos[0], pos[3], uv[0], uv[1] },
        { pos[0], pos[1], uv[0], uv[3] },
        { pos[2], pos[1], uv[2], uv[3] },

        { pos[0], pos[3], uv[0], uv[1] },
        { pos[2], pos[1], uv[2], uv[3] },
        { pos[2], pos[3], uv[2], uv[1] }
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 4 * 6, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
