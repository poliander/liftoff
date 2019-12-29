/*
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

#include "model.hpp"

Model::Model(shared_ptr<Object> o, shared_ptr<Texture> t, shared_ptr<Shader> s) : object(o), texture(t), shader(s) {
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glGenBuffers(4, vertexArrayBuffers);

    // position
    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[VAB_POSITIONS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(object->positions[0]) * object->positions.size(), &object->positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // normal
    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[VAB_NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(object->normals[0]) * object->normals.size(), &object->normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // uv
    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[VAB_TEXCOORDS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(object->texCoords[0]) * object->texCoords.size(), &object->texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[VAB_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(object->indices[0]) * object->indices.size(), &object->indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Model::~Model() {
    glDeleteBuffers(4, vertexArrayBuffers);
    glDeleteVertexArrays(1, &vertexArrayObject);
}

void Model::draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec4 color) {
    shader->bind();
    texture->bind();

    shader->update(UNI_MODEL,      model);
    shader->update(UNI_VIEW,       view);
    shader->update(UNI_PROJECTION, projection);
    shader->update(UNI_COLOR,      color);

    glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, object->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader->unbind();
}
