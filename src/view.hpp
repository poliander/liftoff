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

#include "definitions.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using std::make_unique;
using std::unique_ptr;

class View {
    public:
        View(uint8_t t, glm::mat4 p);
        virtual ~View();

        glm::mat4 getProjection();

        // perspective transformation (3D)

        static unique_ptr<View> createPerspective(float f, float a, float zNear, float zFar);

        void setCamera(
            float px, float py, float pz,
            float tx, float ty, float tz,
            float ux, float uy, float uz
        );

        glm::mat4 getCamera();
        glm::vec3 getCameraPos();

        glm::mat4 transform(
            float px, float py, float pz,
            float rx, float ry, float rz,
            float sx, float sy, float sz
        );

        glm::mat4 getModel(
            float px, float py, float pz,
            float rx, float ry, float rz,
            float sx, float sy, float sz
        );

        // orthographic transformation (2D)

        static unique_ptr<View> createOrthographic(float x1, float x2, float y1, float y2);

        glm::mat4 transform(
            float px, float py,
            float sx, float sy
        );

        glm::mat4 getModel(
            float px, float py,
            float sx, float sy
        );

    private:
        uint8_t type;

        glm::mat4 projection;
        glm::mat4 view;

        glm::vec3 camera_pos;
        glm::vec3 camera_target;
        glm::vec3 camera_up;
};
