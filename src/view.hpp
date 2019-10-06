#pragma once

#include "definitions.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class View
{
    public:
        View();
        virtual ~View();

        void perspective(
            float fov,
            float aspect,
            float zNear,
            float zFar
        );

        void lookAt(
            float camera_pos_x,    float camera_pos_y,    float camera_pos_z,
            float camera_target_x, float camera_target_y, float camera_target_z,
            float camera_up_x,     float camera_up_y,     float camera_up_z
        );

        glm::mat4 transform(
            float px, float py, float pz,
            float rx, float ry, float rz,
            float sx, float sy, float sz
        );

    private:
        glm::vec3 camera_pos;
        glm::vec3 camera_target;
        glm::vec3 camera_up;

        glm::mat4 projection;
        glm::mat4 view;
};
