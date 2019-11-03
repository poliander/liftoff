#pragma once

#include "definitions.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class View
{
    public:
        View();
        virtual ~View();

        // perspective transformation (3D)

        void      initPerspective(float f, float a, float zNear, float zFar);
        glm::mat4 getPerspective();

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

        void      initOrthographic(float x1, float y1, float x2, float y2);
        glm::mat4 getOrthographic();

        glm::mat4 transform(
            float px, float py,
            float sx, float sy
        );

        glm::mat4 getModel(
            float px, float py,
            float sx, float sy
        );

    private:
        glm::mat4 projection[2];
        glm::mat4 view;

        glm::vec3 camera_pos;
        glm::vec3 camera_target;
        glm::vec3 camera_up;
};
