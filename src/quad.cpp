/*
 *  LIFT-OFF: BEYOND GLAXIUM
 *  Copyright (C) 2019 René Pollesch <poliander@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "quad.hpp"

Quad::Quad() {
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

Quad::~Quad() {
}

void Quad::setPosition(glm::vec4 p) {
    pos = p;
    changed = true;
}

void Quad::setTextureCoordinates(glm::vec4 u) {
    uv = u;
    changed = true;
}

void Quad::update() {
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

    changed = false;
}

void Quad::draw() {
    if (changed) update();

    glBindVertexArray(vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
