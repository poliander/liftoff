#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class View
{
    public:
        View();
        virtual ~View();

        void setup(float fov, float aspect, float zNear, float zFar);

        glm::mat4 transform(
            float px, float py, float pz,
            float rx, float ry, float rz,
            float sx, float sy, float sz
        );

    private:
        glm::vec3 camera_pos;
        glm::vec3 camera_up;
        glm::vec3 camera_forward;

        glm::mat4 projection;
        glm::mat4 view_projection;
};
