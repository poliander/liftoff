#include "model.hpp"

Model::Model(const char *txt, const char *obj)
{
    texture = make_shared<Texture>(txt, true);
    load(obj);
}

Model::~Model()
{
    memFree();
}

bool Model::load(const char *filename)
{
    FILE *fp;
    int i, j;

    fp = fopen(filename, "r");

    if (!fp) {
        return false;
    }

    if (!loadFirstPass(fp)) {
        fclose(fp);
        return false;
    }

    rewind(fp);

    if (!memAllocate()) {
        fclose(fp);
        memFree();
        return false;
    }

    if (!loadSecondPass(fp)) {
        fclose(fp);
        memFree();
        return false;
    }

    fclose(fp);

    list = glGenLists(1);

    glNewList(list, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, *texture);

    for (i = 0; i < mesh.num_faces; i++) {
        glBegin(mesh.faces[i].type);

        for (j = 0; j < mesh.faces[i].num_elems; ++j) {
            if (mesh.has_texCoords) {
                glTexCoord3fv(mesh.texCoords[mesh.faces[i].uvw_indices[j]].uvw);
            }

            if (mesh.has_normals) {
                glNormal3fv(mesh.normals[mesh.faces[i].norm_indices[j]].ijk);
            }

            glVertex4fv(mesh.vertices[mesh.faces[i].vert_indices[j]].xyzw);
        }

        glEnd();
    }

    glEndList();

    return true;
}

bool Model::loadFirstPass(FILE *fp)
{
    int v, t, n;
    char buf[256];

    while (!feof(fp)) {
        fgets (buf, sizeof (buf), fp);

        switch (buf[0]) {
            case 'v': // vertex
                if (buf[1] == ' ') {
                    mesh.num_verts++;
                } else if (buf[1] == 't') {
                    // texture coordinate
                    mesh.num_texCoords++;
                } else if (buf[1] == 'n') {
                    // normal
                    mesh.num_normals++;
                } else {
                    // errors ignored
                }
                break;

            case 'f': // face
                if (sscanf (buf + 2, "%d/%d/%d", &v, &n, &t) == 3) {
                    mesh.num_faces++;
                    mesh.has_texCoords = 1;
                    mesh.has_normals = 1;
                } else if (sscanf (buf + 2, "%d//%d", &v, &n) == 2) {
                    mesh.num_faces++;
                    mesh.has_texCoords = 0;
                    mesh.has_normals = 1;
                } else if (sscanf (buf + 2, "%d/%d", &v, &t) == 2) {
                    mesh.num_faces++;
                    mesh.has_texCoords = 1;
                    mesh.has_normals = 0;
                } else if (sscanf (buf + 2, "%d", &v) == 1) {
                    mesh.num_faces++;
                    mesh.has_texCoords = 0;
                    mesh.has_normals = 0;
                } else {
                    // errors ignored
                }
                break;

            case 'g': // groups ignored
                break;
        }
    }

    if (
        (mesh.has_texCoords && !mesh.num_texCoords) ||
        (mesh.has_normals   && !mesh.num_normals)
    ) {
        // contradiction between collected info
        return false;
    }

    if (!mesh.num_verts) {
        // no vertex
        return false;
    }

    return true;
}

bool Model::loadSecondPass(FILE *fp)
{
    struct obj_vertex_t *pvert = mesh.vertices;
    struct obj_texCoord_t *puvw = mesh.texCoords;
    struct obj_normal_t *pnorm = mesh.normals;
    struct obj_face_t *pface = mesh.faces;

    char buf[128], *pbuf;
    int i;

    while (!feof (fp)) {
        fgets (buf, sizeof (buf), fp);

        switch (buf[0]) {
            case 'v':
                if (buf[1] == ' ') {
                    // vertex
                    if (sscanf (buf + 2, "%f %f %f %f",
                      &pvert->xyzw[0], &pvert->xyzw[1],
                      &pvert->xyzw[2], &pvert->xyzw[3]) != 4) {
                        if (sscanf (buf + 2, "%f %f %f", &pvert->xyzw[0],
                          &pvert->xyzw[1], &pvert->xyzw[2] ) != 3) {
                              // vertex data read error
                            return false;
                        } else {
                            pvert->xyzw[3] = 1.0;
                        }
                    }

                    pvert++;
                } else if (buf[1] == 't') {
                    // texture coordinates
                    if (sscanf (buf + 2, "%f %f %f", &puvw->uvw[0],
                      &puvw->uvw[1], &puvw->uvw[2]) != 3) {
                        if (sscanf (buf + 2, "%f %f", &puvw->uvw[0],
                          &puvw->uvw[1]) != 2) {
                            if (sscanf (buf + 2, "%f", &puvw->uvw[0]) != 1) {
                                // tex coordinate read error
                                return false;
                            } else {
                                puvw->uvw[1] = 0.0;
                                puvw->uvw[2] = 0.0;
                            }
                        } else {
                            puvw->uvw[2] = 0.0;
                        }
                    }

                    puvw++;
                } else if (buf[1] == 'n') {
                    // normal
                    if (sscanf (buf + 2, "%f %f %f", &pnorm->ijk[0],
                      &pnorm->ijk[1], &pnorm->ijk[2]) != 3) {
                        // normal coordinate read error
                        return false;
                      }

                    pnorm++;
                }
        break;

        case 'f':
            pbuf = buf;
            pface->num_elems = 0;

            // count vertices for this face
            while (*pbuf) {
                if (*pbuf == ' ')
                    pface->num_elems++;

                pbuf++;
            }

            // select primitive type
            if (pface->num_elems < 3) {
                // less than 3 vertices? duh...
                return false;
            } else if (pface->num_elems == 3) {
                pface->type = GL_TRIANGLES;
            } else if (pface->num_elems == 4) {
                pface->type = GL_QUADS;
            } else {
                pface->type = GL_POLYGON;
            }

            // vertices m'alloc
            pface->vert_indices = (int *)malloc (sizeof (int) * pface->num_elems);

            if (mesh.has_texCoords)
                pface->uvw_indices = (int *)malloc (sizeof (int) * pface->num_elems);

            if (mesh.has_normals)
                pface->norm_indices = (int *)malloc (sizeof (int) * pface->num_elems);

            // read face data
            pbuf = buf;
            i = 0;

            for (i = 0; i < pface->num_elems; ++i) {
                pbuf = strchr (pbuf, ' ');
                pbuf++;

                // try to read vertices
                if (sscanf (pbuf, "%d/%d/%d",
                  &pface->vert_indices[i],
                  &pface->uvw_indices[i],
                  &pface->norm_indices[i]) != 3) {
                    if (sscanf (pbuf, "%d//%d", &pface->vert_indices[i],
                      &pface->norm_indices[i]) != 2) {
                        if (sscanf (pbuf, "%d/%d", &pface->vert_indices[i],
                          &pface->uvw_indices[i]) != 2) {
                            sscanf (pbuf, "%d", &pface->vert_indices[i]);
                        }
                    }
                }

                // indices must start at 0
                pface->vert_indices[i]--;

                if (mesh.has_texCoords)
                    pface->uvw_indices[i]--;

                if (mesh.has_normals)
                    pface->norm_indices[i]--;
            }

            pface++;
            break;
        }
    }

    return true;
}

bool Model::memAllocate()
{
    if (mesh.num_verts) {
        mesh.vertices = (struct obj_vertex_t *)
        malloc (sizeof (struct obj_vertex_t) * mesh.num_verts);

        if (!mesh.vertices) {
            return false;
        }
    }

    if (mesh.num_texCoords) {
        mesh.texCoords = (struct obj_texCoord_t *)
        malloc (sizeof (struct obj_texCoord_t) * mesh.num_texCoords);

        if (!mesh.texCoords) {
            return false;
        }
    }

    if (mesh.num_normals) {
        mesh.normals = (struct obj_normal_t *)
        malloc (sizeof (struct obj_normal_t) * mesh.num_normals);

        if (!mesh.normals) {
            return false;
        }
    }

    if (mesh.num_faces) {
        mesh.faces = (struct obj_face_t *)
        calloc (mesh.num_faces, sizeof (struct obj_face_t));

        if (!mesh.faces) {
            return false;
        }
    }

    return true;
}

void Model::memFree()
{
    int i;

    if (mesh.vertices) {
        free(mesh.vertices);
        mesh.vertices = NULL;
    }

    if (mesh.texCoords) {
        free(mesh.texCoords);
        mesh.texCoords = NULL;
    }

    if (mesh.normals) {
        free(mesh.normals);
        mesh.normals = NULL;
    }

    if (mesh.faces) {
        for (i = 0; i < mesh.num_faces; ++i) {
            if (mesh.faces[i].vert_indices)
                free(mesh.faces[i].vert_indices);

            if (mesh.faces[i].uvw_indices)
                free(mesh.faces[i].uvw_indices);

            if (mesh.faces[i].norm_indices)
                free(mesh.faces[i].norm_indices);
        }

        free(mesh.faces);
        mesh.faces = NULL;
    }
}
