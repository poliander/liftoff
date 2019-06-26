#include "view.hpp"

View::View()
{
    camera_pos    = glm::vec3(0.0f,  0.0f,  0.1f);
    camera_target = glm::vec3(0.0f,  0.0f, -1.0f);
    camera_up     = glm::vec3(0.0f,  1.0f,  0.0f);
}

View::~View()
{
}

void View::perspective(float fov, float aspect, float zNear, float zFar)
{
    projection = glm::perspective(fov, aspect, zNear, zFar);
    view_projection = projection * glm::lookAt(camera_pos, camera_pos + camera_target, camera_up);
}

void View::lookAt(
    float camera_pos_x,    float camera_pos_y,    float camera_pos_z,
    float camera_target_x, float camera_target_y, float camera_target_z,
    float camera_up_x,     float camera_up_y,     float camera_up_z
) {
    camera_pos.x    = camera_pos_x;
    camera_pos.y    = camera_pos_y;
    camera_pos.z    = camera_pos_z;

    camera_target.x = camera_target_x;
    camera_target.y = camera_target_y;
    camera_target.z = camera_target_z;

    camera_up.x     = camera_up_x;
    camera_up.y     = camera_up_y;
    camera_up.z     = camera_up_z;

    view_projection = projection * glm::lookAt(camera_pos, camera_pos + camera_target, camera_up);
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
