#include "model.h"

Model::Model() : model(glm::mat4(1.0f)), movementSpeed(MSPEED), rotateSensitivity(MSENSITIVITY), scaleUp(MSCALEUP), scaleDown(MSCALEDOWN), modelScale(1.0f), modelScaleMax(10.0f), modelScaleMin(0.1f)
{

}

 glm::mat4 Model::getModelMatrix()
 {
     return this->model;
 }

void Model::processTrans(Model_Movement direction, float deltaTime)
{
    float velocity = this->movementSpeed * deltaTime;
    glm::vec3 trans;

    if (direction == MFORWARD)
    {
        trans =glm::vec3(0.0f, 0.0f, 1.0f) * velocity;
    }
    else if (direction == MBACKWARD)
    {
        trans =glm::vec3(0.0f, 0.0f, -1.0f) * velocity;
    }
    else if (direction == MLEFT)
    {
        trans =glm::vec3(-1.0f, 0.0f, 0.0f) * velocity;
    }
    else if (direction == MRIGHT)
    {
        trans =glm::vec3(1.0f, 0.0f, 0.0f) * velocity;
    }
    else if (direction == MUP)
    {
        trans =glm::vec3(0.0f, 1.0f, 0.0f) * velocity;    
    }
    else if (direction == MDOWN)
    {
        trans =glm::vec3(0.0f, -1.0f, 0.0f) * velocity;
    }

    this->model = glm::translate(glm::mat4(1.0f), trans);
}

void Model::processRotate(Model_Rotate direction)
{
    float angle = this->rotateSensitivity;
    glm::vec3 axis;

    if (direction == CLOCKWISEX)
    {
        axis =glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else if (direction == ANTICLOCKWISEX)
    {
        angle *= -1;
        axis =glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else if (direction == CLOCKWISEY)
    {
        axis =glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else if (direction == ANTICLOCKWISEY)
    {
        angle *= -1;
        axis =glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else if (direction == CLOCKWISEZ)
    {
        axis =glm::vec3(0.0f, 0.0f, 1.0f);    
    }
    else if (direction == ANTICLOCKWISEZ)
    {
        angle *= -1;
        axis =glm::vec3(0.0f, 0.0f, 1.0f);
    }

    this->model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
}

void Model::processScale(bool isPlus)
{
    float scale = isPlus ? 1.1f : 0.9f;
    scale = this->modelScale * scale;

    if (scale <= this->modelScaleMax && scale >= this->modelScaleMin)
    {
        this->model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f) * scale);
        this->modelScale = scale;
    }
}
