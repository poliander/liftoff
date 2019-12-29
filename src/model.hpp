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

#pragma once

#include <GL/glew.h>

#include <string>
#include <memory>

#include <object.hpp>
#include <shader.hpp>
#include <texture.hpp>

using std::shared_ptr;

enum vbo_buffer_indices_e {
    VAB_INDICES,
    VAB_POSITIONS,
    VAB_TEXCOORDS,
    VAB_NORMALS
};

class State;

class Model {
    public:
        Model(shared_ptr<Object> o, shared_ptr<Texture> t, shared_ptr<Shader> s);
        ~Model();

        void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec4 color);

    protected:
        shared_ptr<Object>  object;
        shared_ptr<Shader>  shader;
        shared_ptr<Texture> texture;

        GLuint              vertexArrayObject;
        GLuint              vertexArrayBuffers[4];
};
