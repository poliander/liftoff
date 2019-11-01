#include "font.hpp"
#include "state.hpp"

Font::Font(const string &filename, shared_ptr<Shader> s) : shader(s)
{
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexArrayBuffer);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Font::~Font()
{
}

void Font::draw(const string& txt, float x, float y, float z, float s, float r, float g, float b, float a)
{
    shader->bind();
    shader->update(UNI_COLOR, glm::vec4(r, g, b, a));

    glBindVertexArray(vertexArray);

    for (char const &c: txt) {
/*
        auto i = glyphs.find(c);

        if (i != glyphs.end()) {
            shader->update(UNI_MVP, View::transform2D(x, y, 0, 0, s, s));

            GLfloat vertices[6][4] = {
                { 0.0, 0.0, tx,     ty + 0.25f },
                { 0.0, 1.0, tx,     ty },
                { 1.0, 1.0, tx + w, ty },

                { 0.0, 0.0, tx,     ty + 0.25f },
                { 1.0, 1.0, tx + w, ty },
                { 1.0, 0.0, tx + w, ty + 0.25f }
            };

            glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x -= s * w * 30;
        }
*/
    }

    shader->unbind();
}
