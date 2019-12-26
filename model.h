#ifndef MODEL_H
#define MODEL_H

#include "gl.h"

using namespace std;

// 物体的移动方向
enum Model_Movement 
{
    MFORWARD,
    MBACKWARD,
    MLEFT,
    MRIGHT,
    MUP,
    MDOWN
};

// 物体的旋转方向
enum Model_Rotate
{
    CLOCKWISEX,
    ANTICLOCKWISEX,
    CLOCKWISEY,
    ANTICLOCKWISEY,
    CLOCKWISEZ,
    ANTICLOCKWISEZ
};

// 物体位置改变的参数
const float MSPEED = 2.5f;
const float MSENSITIVITY = 0.1f;
const float MSCALEUP = 1.1f;
const float MSCALEDOWN = 0.9f;

class Model
{
public:
    Model();

    // 模型矩阵
    glm::mat4 getModelMatrix();

    // 物体位置移动
    void processTrans(Model_Movement direction, float deltaTime);

    // 物体旋转
    void processRotate(Model_Rotate direction);

    // 物体缩放
    void processScale(bool isPlus);

    inline void setmodelScaleMax(float modelScaleMax);
    inline void setmodelScaleMin(float modelScaleMax);
    inline void setTrans(glm::vec3 pos);
    inline void setScale(glm::vec3 pos);

private:
    glm::mat4 model;

    float movementSpeed;
    float rotateSensitivity;
    float scaleUp;
    float scaleDown;
    float modelScale;                
    float modelScaleMax;      
    float modelScaleMin;        
};

void Model::setmodelScaleMax(float modelScaleMax)
{
    this->modelScaleMax = modelScaleMax > 1.0f ? (modelScaleMax < 10.0f ?  modelScaleMax : 10.0f) : 1.0f;
}

void Model::setmodelScaleMin(float modelScaleMax)
{
    this->modelScaleMax = modelScaleMax < 1.0f ? (modelScaleMax > 0.1f ?  modelScaleMax : 0.1f) : 1.0f;
}

void Model::setTrans(glm::vec3 pos)
{
    this->model = glm::translate(glm::mat4(1.0f), pos);
}

void Model::setScale(glm::vec3 scale)
{
    this->model = glm::scale(this->model, scale);
}

#endif // MODEL_H
