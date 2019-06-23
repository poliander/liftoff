#include "mesh.hpp"

Mesh::Mesh(string filename)
{
    load(filename);
}

Mesh::~Mesh()
{
    memFree();
}

void Mesh::load(string filename)
{
    int i, j;

    FILE *fp = fopen(filename.c_str(), "r");

    if (!fp) {
        return;
    }

    if (!loadFirstPass(fp)) {
        fclose(fp);
        return;
    }

    rewind(fp);

    if (!memAllocate()) {
        fclose(fp);
        memFree();
        return;
    }

    if (!loadSecondPass(fp)) {
        fclose(fp);
        memFree();
        return;
    }

    fclose(fp);
}

bool Mesh::loadFirstPass(FILE *fp)
{
    int v, t, n;
    char buf[256];

    while (!feof(fp)) {
        fgets (buf, sizeof (buf), fp);

        switch (buf[0]) {
            case 'v': // vertex
                if (buf[1] == ' ') {
                    num_verts++;
                } else if (buf[1] == 't') {
                    // texture coordinate
                    num_texCoords++;
                } else if (buf[1] == 'n') {
                    // normal
                    num_normals++;
                } else {
                    // errors ignored
                }
                break;

            case 'f': // face
                if (sscanf (buf + 2, "%d/%d/%d", &v, &n, &t) == 3) {
                    num_faces++;
                    has_texCoords = 1;
                    has_normals = 1;
                } else if (sscanf (buf + 2, "%d//%d", &v, &n) == 2) {
                    num_faces++;
                    has_texCoords = 0;
                    has_normals = 1;
                } else if (sscanf (buf + 2, "%d/%d", &v, &t) == 2) {
                    num_faces++;
                    has_texCoords = 1;
                    has_normals = 0;
                } else if (sscanf (buf + 2, "%d", &v) == 1) {
                    num_faces++;
                    has_texCoords = 0;
                    has_normals = 0;
                } else {
                    // errors ignored
                }
                break;

            case 'g': // groups ignored
                break;
        }
    }

    if (
        (has_texCoords && !num_texCoords) ||
        (has_normals   && !num_normals)
    ) {
        // contradiction between collected info
        return false;
    }

    if (!num_verts) {
        // no vertex
        return false;
    }

    return true;
}

bool Mesh::loadSecondPass(FILE *fp)
{
    struct obj_vertex_t *pvert = vertices;
    struct obj_texCoord_t *puvw = texCoords;
    struct obj_normal_t *pnorm = normals;
    struct obj_face_t *pface = faces;

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

            if (has_texCoords)
                pface->uvw_indices = (int *)malloc (sizeof (int) * pface->num_elems);

            if (has_normals)
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

                if (has_texCoords)
                    pface->uvw_indices[i]--;

                if (has_normals)
                    pface->norm_indices[i]--;
            }

            pface++;
            break;
        }
    }

    return true;
}

bool Mesh::memAllocate()
{
    if (num_verts) {
        vertices = (struct obj_vertex_t *)malloc(sizeof(struct obj_vertex_t) * num_verts);

        if (!vertices) {
            return false;
        }
    }

    if (num_texCoords) {
        texCoords = (struct obj_texCoord_t *)malloc(sizeof(struct obj_texCoord_t) * num_texCoords);

        if (!texCoords) {
            return false;
        }
    }

    if (num_normals) {
        normals = (struct obj_normal_t *)malloc(sizeof(struct obj_normal_t) * num_normals);

        if (!normals) {
            return false;
        }
    }

    if (num_faces) {
        faces = (struct obj_face_t *)calloc(num_faces, sizeof(struct obj_face_t));

        if (!faces) {
            return false;
        }
    }

    return true;
}

void Mesh::memFree()
{
    int i;

    if (vertices) {
        free(vertices);
        vertices = nullptr;
    }

    if (texCoords) {
        free(texCoords);
        texCoords = nullptr;
    }

    if (normals) {
        free(normals);
        normals = nullptr;
    }

    if (faces) {
        for (i = 0; i < num_faces; ++i) {
            if (faces[i].vert_indices)
                free(faces[i].vert_indices);

            if (faces[i].uvw_indices)
                free(faces[i].uvw_indices);

            if (faces[i].norm_indices)
                free(faces[i].norm_indices);
        }

        free(faces);
        faces = nullptr;
    }
}
