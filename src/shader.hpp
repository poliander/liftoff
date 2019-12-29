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

#pragma once

#include <GL/glew.h>

#include <fstream>
#include <string>

#include <glm/glm.hpp>

using std::ifstream;
using std::string;

enum uniforms_e {
    UNI_MODEL,
    UNI_VIEW,
    UNI_PROJECTION,
    UNI_MVP,
    UNI_COLOR
};

class Shader {
    public:
        Shader(const string vsFilename, const string fsFilename);
        ~Shader();

        void   bind();
        void   unbind();
        void   update(uint8_t id, glm::mat4 m);
        void   update(uint8_t id, glm::vec2 v);
        void   update(uint8_t id, glm::vec4 v);

    private:
        GLuint program;
        GLuint shaders[2];
        GLuint uniforms[5];

        string load(const string& filename);
        GLuint create(const string& text, unsigned int type);
};
