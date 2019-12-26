#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), viewZoom(VIEWZOOM), viewZoomMax(45.0f), viewZoomMin(1.0f)
{
    this->position = position;
    this->worldUp = worldUp;

    this->yaw = yaw;
    this->pitch = pitch;

    this->updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float wUpX, float wUpY, float wUpZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), viewZoom(VIEWZOOM), viewZoomMax(45.0f), viewZoomMin(1.0f)
{
    this->position = glm::vec3(posX, posY, posZ);
    this->worldUp = glm::vec3(wUpX, wUpY, wUpZ);

    this->yaw = yaw;
    this->pitch = pitch;

    this->updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(glm::radians(this->viewZoom), float(SCR_WIDTH) / float(SCR_HEIGHT), nearClipPlane, farClipPlane);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = this->movementSpeed * deltaTime;
    if (direction == FORWARD)
    {
        this->position += this->front * velocity;
    }
    else if (direction == BACKWARD)
    {
        this->position -= this->front * velocity;
    }
    else if (direction == LEFT)
    {
        this->position -= this->right * velocity;
    }
    else if (direction == RIGHT)
    {
        this->position += this->right * velocity;
    }
    else if (direction == UP)
    {
        this->position -= this->up * velocity;
    }
    else if (direction == DOWN)
    {
        this->position += this->up * velocity;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;

    this->yaw   += xoffset;
    this->pitch += yoffset;

    if (constrainPitch)
    {
        if (this->pitch > PITCHMAX)
        {
            this->pitch = PITCHMAX;
        }
        else if (this->pitch < PITCHMIN)
        {
            this->pitch = PITCHMIN;
        }
    }

    this->updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    if (this->viewZoom >= this->viewZoomMin && this->viewZoom <= this->viewZoomMax)
    {
        this->viewZoom -= yoffset;
    }
    else if (this->viewZoom <= this->viewZoomMin)
    {
        this->viewZoom = this->viewZoomMin;
    }
    else if (this->viewZoom >= this->viewZoomMax)
    {
        this->viewZoom = this->viewZoomMax;
    }
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    // 标准化、以及注意叉乘方向
    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));  
    this->up    = glm::normalize(glm::cross(this->right, this->front));
}
