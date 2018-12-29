#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glut.h>
#include <SDL_image.h>

#include "state.hpp"

// vector
typedef float vec3_t[3];
typedef float vec4_t[4];

// vertex
struct obj_vertex_t {
	vec4_t xyzw;
};

// texture coordinates
struct obj_texCoord_t {
	vec3_t uvw;
};

// normals
struct obj_normal_t {
	vec3_t ijk;
};

struct obj_face_t {
	GLenum type;			   // primitive type
	int num_elems;			   // number of vertices

	int *vert_indices;		   // vertex indices
	int *uvw_indices;		   // texture coordinate indices
	int *norm_indices;		   // normal vector indices
};

struct obj_model_t {
	int num_verts;                     // number of vertices
	int num_texCoords;                 // number of texture coords.
	int num_normals;                   // number of normal vectors
	int num_faces;                     // number of polygons

	int has_texCoords;                 // has texture coordinates?
	int has_normals;                   // has normal vectors?

	struct obj_vertex_t *vertices;     // vertex list
	struct obj_texCoord_t *texCoords;  // tex. coord. list
	struct obj_normal_t *normals;      // normal vector list
	struct obj_face_t *faces;          // model's polygons
};

class Object {
	public:
		Object(State* sptr);
		~Object();

		GLuint		textures[4];

		bool 		load(const char* datadir, const char* filename);
		void		unload();

		void		move(int oid);
		void		draw(int oid);
		void		drawCrosshair(int oid, float r, float g, float b);

		void		setPos(int oid, float px, float py, float pz);

		void		accelerateX(int oid, float ax);
		void		accelerateY(int oid, float ay);
		void		accelerateZ(int oid, float az);

		void		impulse(int oid, float ix, float iy, float iz);

		void		setRot(int oid, float rx, float ry, float rz);
		float		getRotX(int oid);
		float		getRotY(int oid);
		float		getRotZ(int oid);

		void		setSpin(int oid, float sx, float sy, float sz);
		float		getSpinX(int oid);
		float		getSpinY(int oid);
		float		getSpinZ(int oid);

	protected:
		State* state;

		obj_model_t	model;
		GLuint		model_list;

		float		alpha;
		float		shininess;
		float		brightness;

		bool		_load_OBJ_firstPass(FILE *fp);
		bool		_load_OBJ_secondPass(FILE *fp);
		bool		_load_OBJ_malloc();
		void		_load_OBJ_freemem();
};

#endif
