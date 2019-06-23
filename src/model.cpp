#include "model.hpp"

Model::Model(shared_ptr<Mesh> m, shared_ptr<Texture> t, shared_ptr<Shader> s) :
    mesh(m),
    texture(t),
    shader(s)
{
    list = glGenLists(1);

    glNewList(list, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, *texture);

    for (int i = 0; i < mesh->num_faces; i++) {
        glBegin(mesh->faces[i].type);

        for (int j = 0; j < mesh->faces[i].num_elems; ++j) {
            if (mesh->has_texCoords) {
                glTexCoord3fv(mesh->texCoords[mesh->faces[i].uvw_indices[j]].uvw);
            }

            if (mesh->has_normals) {
                glNormal3fv(mesh->normals[mesh->faces[i].norm_indices[j]].ijk);
            }

            glVertex4fv(mesh->vertices[mesh->faces[i].vert_indices[j]].xyzw);
        }

        glEnd();
    }

    glEndList();
}

Model::~Model()
{
    glDeleteLists(list, 1);
}
