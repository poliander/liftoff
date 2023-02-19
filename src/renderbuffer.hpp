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

#include <memory>

#include "state.hpp"
#include "framebuffer.hpp"

class Renderbuffer {
    public:
        explicit Renderbuffer(State* s);
        ~Renderbuffer();

        void bind();
        void unbind();
        void clear();
        void blit();
        void draw(glm::mat4 mvp, glm::vec4 color);

    protected:
        State*                  state;

        unique_ptr<Framebuffer> framebuffer;

        GLuint                  renderbuffer;
        GLuint                  renderbufferColor;
        GLuint                  renderbufferDepth;
};
