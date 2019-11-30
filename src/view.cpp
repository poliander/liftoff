#include "view.hpp"

View::View(unsigned short t, glm::mat4 p)
{
    type = t;
    projection = p;
}

View::~View()
{
}

/*
 * Create a perspective projection view
 */
unique_ptr<View> View::createPerspective(float f, float a, float zNear, float zFar)
{
    return make_unique<View>(P_PERSPECTIVE, glm::perspective(glm::radians(f), a, zNear, zFar));
}

/*
 * Create a orthographic projection view
 */
unique_ptr<View> View::createOrthographic(float x1, float y1, float x2, float y2)
{
    return make_unique<View>(P_ORTHOGRAPHIC, glm::ortho(x1, x2, y1, y2));
}

/*
 * Get projection matrix
 */
glm::mat4 View::getProjection()
{
    return projection;
}

/*
 * Update camera (view)
 */
void View::setCamera(
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

    view = glm::lookAt(camera_pos, camera_pos + camera_target, camera_up);
}

glm::mat4 View::getCamera()
{
    return view;
}

/*
 * Get camera position
 */
glm::vec3 View::getCameraPos()
{
    return camera_pos;
}

/*
 * Perspective transformation (3D)
 */
glm::mat4 View::transform(
    float px, float py, float pz,
    float rx, float ry, float rz,
    float sx, float sy, float sz
) {
    return getProjection() * getCamera() * getModel(px, py, pz, rx, ry, rz, sx, sy, sz);
}

glm::mat4 View::getModel(
    float px, float py, float pz,
    float rx, float ry, float rz,
    float sx, float sy, float sz
) {
    glm::mat4 rotX = glm::rotate(glm::radians(-rx), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotY = glm::rotate(glm::radians(-ry), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotZ = glm::rotate(glm::radians(-rz), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 posMat = glm::translate(glm::vec3(-px, -py, pz));
    glm::mat4 rotMat = rotX * rotY * rotZ;
    glm::mat4 scaleMat = glm::scale(glm::vec3(sx, sy, sz));

    glm::mat4 model = posMat * rotMat * scaleMat;

    return model;
}

/*
 * Orthographic transformation (2D)
 */
glm::mat4 View::transform(
    float px, float py,
    float sx, float sy
) {
    return getProjection() * getModel(px, py, sx, sy);
}

glm::mat4 View::getModel(
    float px, float py,
    float sx, float sy
) {
    glm::mat4 model = glm::translate(glm::vec3(px, py, 0)) * glm::scale(glm::vec3(sx, sy, 1.0f));

    return model;
}
