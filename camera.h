#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "def.h"
#include "gl.h"

using namespace std;

// 摄像机的移动方向
enum Camera_Movement 
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// 摄像机视角、视野的默认参数
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float PITCHMAX = 89.0f;
const float PITCHMIN = -89.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float VIEWZOOM = 25.0f;

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float wUpX, float wUpY, float wUpZ, float yaw, float pitch);

    // 观察矩阵
    glm::mat4 getViewMatrix();

    // 投影矩阵
    glm::mat4 getProjectionMatrix();

    // 摄像机位置移动
    void processKeyboard(Camera_Movement direction, float deltaTime);

    // 摄像机视角改变
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // 摄像机视野改变
    void processMouseScroll(float yoffset);

    inline void setViewZoomMax(float viewZoomMax);
    inline void setViewZoomMin(float viewZoomMax);

    inline glm::vec3 getPosition();
    inline glm::vec3 getFront();

private:
    // 摄像机属性
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // 欧拉角
    float yaw;
    float pitch;

    // 摄像机参数
    float movementSpeed;
    float mouseSensitivity;
    float viewZoom;
    float viewZoomMax;
    float viewZoomMin;

    // 更新摄像机坐标系
    void updateCameraVectors();
};

void Camera::setViewZoomMax(float viewZoomMax)
{
    this->viewZoomMax = viewZoomMax > 1.0f ? (viewZoomMax < 10.0f ?  viewZoomMax : 10.0f) : 1.0f;
}

void Camera::setViewZoomMin(float viewZoomMax)
{
    this->viewZoomMax = viewZoomMax < 1.0f ? (viewZoomMax > 0.1f ?  viewZoomMax : 0.1f) : 1.0f;
}

glm::vec3 Camera::getPosition()
{
    return this->position;
}

glm::vec3 Camera::getFront()
{
    return this->front;
}

#endif // CAMERA_H
