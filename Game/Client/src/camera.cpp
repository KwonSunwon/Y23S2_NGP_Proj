#include "camera.h"

// Camera defualt constructor
Camera::Camera()
{
    pitch = 0.f;
    yaw = -90.f;
    roll = 0.f;

    eye = glm::vec3(0.0f, 0.f, 3.f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    fovy = 45.0f;
    aspect = 800.0f / 800.0f;

    zNear = 0.1f;
    zFar = 100.0f;

    left = -1.0f;
    right = 1.0f;
    bottom = -1.0f;
    top = 1.0f;
}

Camera::Camera(int width, int height)
{
    eye = glm::vec3(0.0f, 0.f, 3.f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    fovy = 45.0f;
    aspect = width / height;

    zNear = 0.1f;
    zFar = 100.0f;

    pitch = 0.f;
    yaw = -90.f;
    roll = 0.f;
}

void Camera::setCamera(GLuint shaderProgramID, int type) // 0 = perspective, 1 = ortho
{
    glm::vec3 _target;
    _target.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    _target.y = sin(glm::radians(pitch));
    _target.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    target = glm::normalize(_target);

    glm::mat4 rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(roll), target);
    glm::vec3 _up = glm::vec3(rollMat * glm::vec4(up, 1.0f));
    _up = glm::normalize(_up);

    view = glm::lookAt(eye, eye + target, _up);

    projection = glm::perspective(fovy, aspect, zNear, zFar);
    ortho = glm::ortho(left, right, bottom, top, zNear, zFar);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    if (type == 0)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    }
    else if (type == 1)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(ortho));
    }
}

void Camera::rolling(float angle, int direction)
{
    this->roll += angle * direction;
}

void Camera::setEye(glm::vec3 eye) { this->eye = eye; }
void Camera::setTarget(glm::vec3 target) { this->target = target; }
void Camera::setUp(glm::vec3 up) { this->up = up; }

glm::vec3 Camera::getEye() { return eye; }
glm::vec3 Camera::getTarget() { return target; }
glm::vec3 Camera::getUp() { return up; }

void Camera::setPitch(float pitch) { this->pitch = pitch; }
void Camera::setYaw(float yaw) { this->yaw = yaw; }
void Camera::setRoll(float roll) { this->roll = roll; }

float Camera::getPitch() { return pitch; }
float Camera::getYaw() { return yaw; }
float Camera::getRoll() { return roll; }

void Camera::setFovy(float fovy) { this->fovy = fovy; }
void Camera::setAspect(float aspect) { this->aspect = aspect; }
void Camera::setzNear(float zNear) { this->zNear = zNear; }
void Camera::setzFar(float zFar) { this->zFar = zFar; }

void Camera::setLeft(float left) { this->left = left; }
void Camera::setRight(float right) { this->right = right; }
void Camera::setBottom(float bottom) { this->bottom = bottom; }
void Camera::setTop(float top) { this->top = top; }

glm::mat4 Camera::getView() { return view; }
glm::mat4 Camera::getProjection() { return projection; }
glm::mat4 Camera::getOrtho() { return ortho; }