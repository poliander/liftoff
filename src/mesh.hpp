#pragma once

#include <stdio.h>
#include <cstring>
#include <string>
#include <memory>

#include <GL/glew.h>
#include <GL/glext.h>

using namespace std;

// vector
typedef float vec3_t[3];
typedef float vec4_t[4];

// vertex
struct obj_vertex_t
{
    vec4_t xyzw;
};

// texture coordinates
struct obj_texCoord_t
{
    vec3_t uvw;
};

// normals
struct obj_normal_t
{
    vec3_t ijk;
};

// faces
struct obj_face_t
{
    GLenum type;                            // primitive type
    int num_elems;                          // number of vertices

    int *vert_indices;                      // vertex indices
    int *uvw_indices;                       // texture coordinate indices
    int *norm_indices;                      // normal vector indices
};

class Mesh
{
    public:
        Mesh(string filename);
        ~Mesh();

        int num_verts = 0;                  // number of vertices
        int num_texCoords = 0;              // number of texture coords.
        int num_normals = 0;                // number of normal vectors
        int num_faces = 0;                  // number of polygons

        int has_texCoords = 0;              // has texture coordinates?
        int has_normals = 0;                // has normal vectors?

        struct obj_vertex_t *vertices;      // vertex list
        struct obj_texCoord_t *texCoords;   // tex. coord. list
        struct obj_normal_t *normals;       // normal vector list
        struct obj_face_t *faces;           // model's polygons

    protected:
        void load(string filename);
        bool loadFirstPass(FILE *fp);
        bool loadSecondPass(FILE *fp);

        bool memAllocate();
        void memFree();
};
