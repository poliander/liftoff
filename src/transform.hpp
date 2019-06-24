#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "camera.hpp"

struct Transform
{
    public:
        glm::vec3 pos;
        glm::vec3 rot;
        glm::vec3 scale;

        Transform(
            const glm::vec3& pos = glm::vec3(),
            const glm::vec3& rot = glm::vec3(),
            const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f)
        ) {
            this->pos = pos;
            this->rot = rot;
            this->scale = scale;
        }

        inline glm::mat4 getModel() const {
            glm::mat4 posMat = glm::translate(pos);
            glm::mat4 scaleMat = glm::scale(scale);
            glm::mat4 rotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
            glm::mat4 rotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
            glm::mat4 rotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));
            glm::mat4 rotMat = rotX * rotY * rotZ;

            return posMat * rotMat * scaleMat;
        }

        inline glm::mat4 getMVP(const Camera& camera) const {
            return camera.getViewProjection() * getModel();
        }
};
