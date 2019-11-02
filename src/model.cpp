#include "model.hpp"

Model::Model(shared_ptr<Object> o, shared_ptr<Texture> t, shared_ptr<Shader> s) :
    object(o),
    texture(t),
    shader(s)
{
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glGenBuffers(4, vertexArrayBuffers);

    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[VAB_POSITIONS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(object->positions[0]) * object->positions.size(), &object->positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[VAB_TEXCOORDS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(object->texCoords[0]) * object->texCoords.size(), &object->texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[VAB_NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(object->normals[0]) * object->normals.size(), &object->normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[VAB_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(object->indices[0]) * object->indices.size(), &object->indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Model::~Model()
{
    glDeleteBuffers(4, vertexArrayBuffers);
    glDeleteVertexArrays(1, &vertexArrayObject);
}

void Model::draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec4 color)
{
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
