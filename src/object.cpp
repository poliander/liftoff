#include "object.hpp"
#include "state.hpp"

Object::Object(State* sptr) {
    memset(&model, 0, sizeof(struct obj_model_t));
    state = sptr;

    alpha = 1.0f;
    shininess = 4.0f;
    brightness = 1.0f;
}

Object::~Object() {
    _load_OBJ_freemem();
}

bool Object::_load_OBJ_firstPass(FILE *fp) {
    int v, t, n;
    char buf[256];

    while (!feof(fp)) {
        fgets (buf, sizeof (buf), fp);

        switch (buf[0]) {
            case 'v': // vertex
                if (buf[1] == ' ') {
                    model.num_verts++;
                } else if (buf[1] == 't') {
                    // texture coordinate
                    model.num_texCoords++;
                } else if (buf[1] == 'n') {
                    // normal
                    model.num_normals++;
                } else {
                    // errors ignored
                }
                break;

            case 'f': // face
                if (sscanf (buf + 2, "%d/%d/%d", &v, &n, &t) == 3) {
                    model.num_faces++;
                    model.has_texCoords = 1;
                    model.has_normals = 1;
                } else if (sscanf (buf + 2, "%d//%d", &v, &n) == 2) {
                    model.num_faces++;
                    model.has_texCoords = 0;
                    model.has_normals = 1;
                } else if (sscanf (buf + 2, "%d/%d", &v, &t) == 2) {
                    model.num_faces++;
                    model.has_texCoords = 1;
                    model.has_normals = 0;
                } else if (sscanf (buf + 2, "%d", &v) == 1) {
                    model.num_faces++;
                    model.has_texCoords = 0;
                    model.has_normals = 0;
                } else {
                    // errors ignored
                }
                break;

            case 'g': // groups ignored
                break;
        }
    }

    // data ok?
    if ( (model.has_texCoords && !model.num_texCoords) ||
         (model.has_normals && !model.num_normals)) {
        // contradiction between collected info
        return false;
    }

    if (!model.num_verts) {
        // no vertex
        return false;
    }

    return true;
}

bool Object::_load_OBJ_secondPass(FILE *fp) {
    struct obj_vertex_t *pvert = model.vertices;
    struct obj_texCoord_t *puvw = model.texCoords;
    struct obj_normal_t *pnorm = model.normals;
    struct obj_face_t *pface = model.faces;

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

            if (model.has_texCoords)
                pface->uvw_indices = (int *)malloc (sizeof (int) * pface->num_elems);

            if (model.has_normals)
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

                if (model.has_texCoords)
                    pface->uvw_indices[i]--;

                if (model.has_normals)
                    pface->norm_indices[i]--;
            }

            pface++;
            break;
        }
    }

    return true;
}

bool Object::_load_OBJ_malloc() {
    if (model.num_verts) {
        model.vertices = (struct obj_vertex_t *)
        malloc (sizeof (struct obj_vertex_t) * model.num_verts);

        if (!model.vertices) return false;
    }

    if (model.num_texCoords) {
        model.texCoords = (struct obj_texCoord_t *)
        malloc (sizeof (struct obj_texCoord_t) * model.num_texCoords);

        if (!model.texCoords) return false;
    }

    if (model.num_normals) {
        model.normals = (struct obj_normal_t *)
        malloc (sizeof (struct obj_normal_t) * model.num_normals);

        if (!model.normals) return false;
    }

    if (model.num_faces) {
        model.faces = (struct obj_face_t *)
        calloc (model.num_faces, sizeof (struct obj_face_t));

        if (!model.faces) return false;
    }

    return true;
}

void Object::_load_OBJ_freemem() {
    int i;

    if (model.vertices) {
        free (model.vertices);
        model.vertices = NULL;
    }

    if (model.texCoords) {
        free (model.texCoords);
        model.texCoords = NULL;
    }

    if (model.normals) {
        free (model.normals);
        model.normals = NULL;
    }

    if (model.faces) {
        for (i=0; i<model.num_faces; ++i) {
            if (model.faces[i].vert_indices)
                free (model.faces[i].vert_indices);

            if (model.faces[i].uvw_indices)
                free (model.faces[i].uvw_indices);

            if (model.faces[i].norm_indices)
                free (model.faces[i].norm_indices);
        }

        free (model.faces);
        model.faces = NULL;
    }
}

bool Object::load(const char *datadir, const char *filename) {
    FILE *fp;
    int i, j;

    char fname[255];
    sprintf(fname, "%s/obj/%s", datadir, filename);

    fp = fopen(fname, "r");
    if (!fp) return false;

    if (!_load_OBJ_firstPass(fp)) {
        fclose(fp);
        return false;
    }

    rewind(fp);

    if (!_load_OBJ_malloc()) {
        fclose(fp);
        _load_OBJ_freemem();
        return false;
    }

    if (!_load_OBJ_secondPass(fp)) {
        fclose(fp);
        _load_OBJ_freemem();
        return false;
    }

    fclose(fp);

    model_list = glGenLists(1);
    glNewList(model_list, GL_COMPILE); {
        glBindTexture(GL_TEXTURE_2D, textures[0]);

        for (i=0; i<model.num_faces; i++) {
            glBegin (model.faces[i].type);
            for (j = 0; j < model.faces[i].num_elems; ++j) {
                if (model.has_texCoords)
                    glTexCoord3fv (model.texCoords[model.faces[i].uvw_indices[j]].uvw);

                if (model.has_normals)
                    glNormal3fv (model.normals[model.faces[i].norm_indices[j]].ijk);

                glVertex4fv (model.vertices [model.faces[i].vert_indices[j]].xyzw);
            }
            glEnd();
        }

    } glEndList();

    return true;
}

void Object::setPos(int oid, float px, float py, float pz) {
    state->objects[oid].pos_x = px;
    state->objects[oid].pos_y = py;
    state->objects[oid].pos_z = pz;
}

void Object::accelerateX(int oid, float ax) {
    state->objects[oid].t_x = ax;
}

void Object::accelerateY(int oid, float ay) {
    state->objects[oid].t_y = ay;
}

void Object::accelerateZ(int oid, float az) {
    state->objects[oid].t_z = az;
}

/*
 *     object receives impulse
 */
void Object::impulse(int oid, float ix, float iy, float iz) {
    state->objects[oid].a_x += ix;
    state->objects[oid].a_y += iy;
    state->objects[oid].a_z += iz;
}

/*
 * set object rotation
 */
void Object::setRot(int oid, float rx, float ry, float rz) {
    state->objects[oid].rot_x = rx;
    state->objects[oid].rot_y = ry;
    state->objects[oid].rot_z = rz;
}

float Object::getRotX(int oid) {
    return state->objects[oid].rot_x;
}

float Object::getRotY(int oid) {
    return state->objects[oid].rot_y;
}

float Object::getRotZ(int oid) {
    return state->objects[oid].rot_z;
}

void Object::setSpin(int oid, float sx, float sy, float sz) {
    state->objects[oid].rsp_x = sx;
    state->objects[oid].rsp_y = sy;
    state->objects[oid].rsp_z = sz;
}

float Object::getSpinX(int oid) {
    return state->objects[oid].rsp_x;
}

float Object::getSpinY(int oid) {
    return state->objects[oid].rsp_y;
}

float Object::getSpinZ(int oid) {
    return state->objects[oid].rsp_z;
}

void Object::move(int oid) {
    if (state->objects[oid].type == OBJ_TYPE_SCENERY) {
        if (state->objects[oid].id == OBJ_POWERUP_1) {
            state->objects[oid].cnt2 += state->timer_adjustment;
            state->objects[oid].pos_z += state->timer_adjustment * (state->objects[oid].speed + state->objects[state->player].speed) * .25f;
        } else {
            state->objects[oid].pos_z += state->timer_adjustment * (state->objects[oid].speed + state->objects[state->player].speed) * .5f;
        }
    } else {
        state->objects[oid].pos_z += state->timer_adjustment * (state->objects[oid].speed + state->objects[state->player].speed);
    }

    state->objects[oid].rot_x += state->objects[oid].rsp_x * state->timer_adjustment * .1f;
    if (state->objects[oid].rot_x > 360.0f)
        state->objects[oid].rot_x -= 360.0f;

    state->objects[oid].rot_y += state->objects[oid].rsp_y * state->timer_adjustment * .1f;
    if (state->objects[oid].rot_y > 360.0f)
        state->objects[oid].rot_y -= 360.0f;

    state->objects[oid].rot_z += state->objects[oid].rsp_z * state->timer_adjustment * .1f;
    if (state->objects[oid].rot_z > 360.0f)
        state->objects[oid].rot_z -= 360.0f;

    if (state->objects[oid].pos_z > 100.0f) state->remove(oid);
}

void Object::drawCrosshair(int oid, float r, float g, float b) {
    static float da = .85f;
    float alpha = 1.0f;
    float s, rot;

    if (state->objects[oid].type == OBJ_TYPE_SCENERY) return;

    // in radar range?
    if (state->objects[oid].pos_z < -8000.0f) return;

    // cross-hairs fade out after player died
    if (state->objects[state->player].life <= 0) {
        if (da > .01f) {
            da -= state->timer_adjustment * .01f;
        } else {
            da = .0f;
        }
    } else {
        da = .85f;
    }

    s = 150.0f + (state->objects[oid].pos_z + 12500.0f) * .00005f;

    // cross-hair fades out when too near
    if (state->objects[oid].pos_z > -1000.0f) {
        alpha -= .001f * (1000.0f + state->objects[oid].pos_z);
    }

    alpha *= alpha * float(state->global_alpha) * .01f * da;
    if (alpha > 1.0f) alpha = 1.0f;

    glLoadIdentity();

    glRotatef(-state->tilt_x*.025f, 0, 1, 0);
    glRotatef(-state->tilt_y*.025f, 1, 0, 0);

    glBindTexture(GL_TEXTURE_2D, state->texture[T_HUD_3]);
    glTranslatef(
        (state->objects[oid].pos_x - state->cam_x) * E_RELATIVE_MOVEMENT,
        (state->objects[oid].pos_y - state->cam_y) * E_RELATIVE_MOVEMENT,
        state->objects[oid].pos_z
    );
    glPushMatrix();
    if (oid == state->objects[state->player].target) {
        // target locked, colored cross-hair
        glColor4f(r, g, b, alpha);
        glBegin (GL_QUADS);
          glTexCoord2i(0, 0);
          glVertex2f(-s*.75f, -s*.75f);

          glTexCoord2i(1, 0);
          glVertex2f(s*.75f, -s*.75f);

          glTexCoord2i(1, 1);
          glVertex2f(s*.75f, s*.75f);

          glTexCoord2i(0, 1);
          glVertex2f(-s*.75f, s*.75f);
        glEnd();
    } else {
        // target is NOT locked, grey color
        rot = (180.0f/M_PI) * atan( (state->objects[state->player].pos_y - state->objects[oid].pos_y) / (state->objects[state->player].pos_x - state->objects[oid].pos_x) );
        if (state->objects[state->player].pos_x < state->objects[oid].pos_x) {
            rot += 180.0f;
        }

        glColor4f(1, 1, 1, alpha);
        glRotatef(rot, 0, 0, 1);
        glBegin (GL_QUADS);
          glTexCoord2f(.5f, 0);
          glVertex2f(.8f, -s*.75f);

          glTexCoord2f(1.0f, 0);
          glVertex2f(s*.75f, -s*.75f);

          glTexCoord2f(1.0f, 1.0f);
          glVertex2f(s*.75f, s*.75f);

          glTexCoord2f(.5f, 1.0f);
          glVertex2f(.8f, s*.75f);
        glEnd();
        glRotatef(-rot, 0, 0, 1);
    }

    glBegin (GL_QUADS);
      glTexCoord2i(0, 0);
      glVertex2f(-s, -s);

      glTexCoord2i(1, 0);
      glVertex2f(s, -s);

      glTexCoord2i(1, 1);
      glVertex2f(s, s);

      glTexCoord2i(0, 1);
      glVertex2f(-s, s);
    glEnd();

    glPopMatrix();
}

void Object::draw(int oid) {
    float a = float(state->global_alpha) * .01f;
    float s = 1.0f;

    float x = state->objects[oid].pos_x;
    float y = state->objects[oid].pos_y;
    float z = state->objects[oid].pos_z;

    if (state->objects[oid].type == OBJ_TYPE_SCENERY) {
        s = (10000.0f + state->objects[oid].pos_z) * .000033f;
    } else {
        s = (10000.0f + state->objects[oid].pos_z) * .0001f;
    }

    glLoadIdentity();

    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // light setup
    GLfloat col_specular[] = { a, a, a, a };
    glLightfv(GL_LIGHT0, GL_SPECULAR, col_specular);

    GLfloat col_ambient[] = { .25f, .225f, .2f, a };
    glLightfv(GL_LIGHT0, GL_AMBIENT, col_ambient);

    GLfloat col_diffuse[] = { .0f, .0f, .0f, a };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, col_diffuse);

    GLfloat pos_ambient[] = { .5f, .5f, .0f, a };
    glLightfv(GL_LIGHT0, GL_POSITION, pos_ambient);

    glEnable(GL_LIGHT0);

    // material setup
    GLfloat col_emission[] = { .25f*a, .225f*a, .2f*a, a };
    glMaterialfv(GL_FRONT, GL_AMBIENT, col_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, col_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, col_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, col_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glPushMatrix();

    x -= state->cam_x;
    y -= state->cam_y;

    if (state->objects[oid].type == OBJ_TYPE_SCENERY) {
        x *= E_RELATIVE_MOVEMENT * .85f;
        y *= E_RELATIVE_MOVEMENT * .85f;
    } else {
        x *= E_RELATIVE_MOVEMENT;
        y *= E_RELATIVE_MOVEMENT;
    }

    glRotatef(-state->tilt_x*.035f, .0f, 1.0f, .0f);
    glRotatef(-state->tilt_y*.035f, 1.0f, .0f, .0f);

    glTranslatef(x, y, z);

    glRotatef(state->objects[oid].rot_x, 1.0f, .0f, .0f);
    glRotatef(state->objects[oid].rot_y, .0f, 1.0f, .0f);
    glRotatef(state->objects[oid].rot_z, .0f, .0f, 1.0f);

    glScalef(
        state->objects[oid].scale_x * s,
        state->objects[oid].scale_y * s,
        state->objects[oid].scale_z * s
    );

    glCallList(model_list);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_NORMALIZE);
    glDisable(GL_CULL_FACE);
    glPopMatrix();
}
