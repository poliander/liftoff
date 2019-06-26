#include "view.hpp"

View::View()
{
    camera_pos     = glm::vec3(0.0f,  0.0f,  0.1f);
    camera_forward = glm::vec3(0.0f,  0.0f, -1.0f);
    camera_up      = glm::vec3(0.0f,  1.0f,  0.0f);
}

View::~View()
{
}

void View::setup(float fov, float aspect, float zNear, float zFar)
{
    projection = glm::perspective(fov, aspect, zNear, zFar);
    view_projection = projection * glm::lookAt(camera_pos, camera_pos + camera_forward, camera_up);
}

glm::mat4 View::transform(
    float px, float py, float pz,
    float rx, float ry, float rz,
    float sx, float sy, float sz
) {
    glm::mat4 rotX = glm::rotate(rx, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotY = glm::rotate(ry, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotZ = glm::rotate(rz, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 posMat = glm::translate(glm::vec3(px, py, pz));
    glm::mat4 scaleMat = glm::scale(glm::vec3(sx, sy, sz));
    glm::mat4 rotMat = rotX * rotY * rotZ;

    glm::mat4 model = posMat * rotMat * scaleMat;

    return view_projection * model;
}
