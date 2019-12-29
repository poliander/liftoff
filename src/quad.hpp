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
#include <glm/glm.hpp>

class Quad {
    public:
        Quad();
        virtual ~Quad();

        void setPosition(glm::vec4 p);
        void setTextureCoordinates(glm::vec4 t);
        void draw();

    protected:
        glm::vec4 uv;
        glm::vec4 pos;

        bool changed;
        void update();

        GLuint vertexArray;
        GLuint vertexArrayBuffer;
};
