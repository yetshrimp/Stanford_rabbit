#include <QCoreApplication>

#include "def.h"
#include "ply.h"
#include "shader.h"
#include "camera.h"
#include "model.h"

#include <math.h>
#include <stdlib.h>

using namespace std;


Shader * rabbitShader;
Shader * lightShader;
Camera * camera;
Model * rabbitModel;
Model * lightModel;

float deltaTime;                         // 当前帧与上一帧的时间差
float lastFrame;                         // 上一帧的时间

bool firstMouse;                          // 系统刚启动时设置鼠标的初始位置
double lastX;                             // 光标上一次的x轴位置
double lastY;                             // 光标上一次的y轴位置
double xOffset;                           // 光标移动的x轴位置
double yOffset;                           // 光标移动的y轴位置

float lightVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

unsigned int lightIndices[] = {
     0,  1,  2,
     3,  4,  5,
     6,  7,  8,
     9, 10, 11,
    12, 13, 14,
    15, 16, 17,
    18, 19, 20,
    21, 22, 23,
    24, 25, 26,
    27, 28, 29,
    30, 31, 32,
    33, 34, 35
};

//unsigned int lightIndices[] = {
//     0,  1,  2,
//     6,  7,  8,
//    12, 13, 14,
//    18, 19, 20,
//    24, 25, 26,
//    30, 31, 32,
//    36, 37, 48,
//    44, 45, 46,
//    50, 51, 52,
//    56, 57, 58,
//    62, 63, 64,
//    68, 69, 70
//};

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

glm::vec3 pointLightScales[] = {
    glm::vec3(0.2f),
    glm::vec3(0.08f),
    glm::vec3(0.15f),
    glm::vec3(0.1f)
};


void iniGlfw()
{
    // glfw 初始化配置
    // ---------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    // 兼容OS操作系统
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

}

// 配置 rabbitVAO rabbitVBO rabbitEBO
void confRabbitABB(unsigned int &VAO, unsigned int &VBO, unsigned int EBO, float vertices[], unsigned int indices[])
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 为VBO, VAO, EBO绑定相关对象、提供数据、设置属性
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 每个顶点属性从一个VBO管理的内存中获得它的数据，而具体是从哪个VBO（程序中可以有多个VBO）获取则是通过在调用glVertexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决定的。由于在调用glVertexAttribPointer之前绑定的是先前定义的VBO对象，顶点属性0现在会链接到它的顶点数据。
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 允许解绑但非必须
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// 配置 lightVAO lightVBO lightEBO
void confLightABB(unsigned int &VAO, unsigned int &VBO, unsigned int EBO, float vertices[], unsigned int indices[])
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 为VBO, VAO, EBO绑定相关对象、提供数据、设置属性
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 每个顶点属性从一个VBO管理的内存中获得它的数据，而具体是从哪个VBO（程序中可以有多个VBO）获取则是通过在调用glVertexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决定的。由于在调用glVertexAttribPointer之前绑定的是先前定义的VBO对象，顶点属性0现在会链接到它的顶点数据。
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // 允许解绑但非必须
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void updateTime()
{
    float currentFrame = float(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

// glfw 错误处理机制
// ------------------------
void error_callback(GLFWwindow* window, const char* description)
{
    cout << "GLFW ERROR " << description << endl;
    glfwSetWindowShouldClose(window, true);
}

// glfw 窗口改变时触发
// ---------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 可视区域需要匹配窗口大小
    glViewport(0, 0, width, height);
}

// glfw 有键盘操作时触发
// ------------------------------
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS) {
        return;
    }

    switch (key)
    {
    case GLFW_KEY_ESCAPE :
        glfwSetWindowShouldClose(window, true);
        break;

    case GLFW_KEY_A :
        rabbitModel->processTrans(Model_Movement::MLEFT, deltaTime);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_D :
        rabbitModel->processTrans(Model_Movement::MRIGHT, deltaTime);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_W :
        rabbitModel->processTrans(Model_Movement::MUP, deltaTime);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_S:
        rabbitModel->processTrans(Model_Movement::MDOWN, deltaTime);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_Q :
        rabbitModel->processTrans(Model_Movement::MFORWARD, deltaTime);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_E :
        rabbitModel->processTrans(Model_Movement::MBACKWARD, deltaTime);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_F :
        rabbitModel->processRotate(Model_Rotate::CLOCKWISEY);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_H :
        rabbitModel->processRotate(Model_Rotate::ANTICLOCKWISEY);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_T :
        rabbitModel->processRotate(Model_Rotate::CLOCKWISEX);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_G :
        rabbitModel->processRotate(Model_Rotate::ANTICLOCKWISEX);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_R :
        rabbitModel->processRotate(Model_Rotate::CLOCKWISEZ);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_Y :
        rabbitModel->processRotate(Model_Rotate::ANTICLOCKWISEZ);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_J :
        camera->processKeyboard(Camera_Movement::LEFT, deltaTime);
        rabbitShader->setMat4("view", camera->getViewMatrix());
        break;

    case GLFW_KEY_L :
        camera->processKeyboard(Camera_Movement::RIGHT, deltaTime);
        rabbitShader->setMat4("view", camera->getViewMatrix());
        break;

    case GLFW_KEY_I :
        camera->processKeyboard(Camera_Movement::UP, deltaTime);
        rabbitShader->setMat4("view", camera->getViewMatrix());
        break;

    case GLFW_KEY_K:
        camera->processKeyboard(Camera_Movement::DOWN, deltaTime);
        rabbitShader->setMat4("view", camera->getViewMatrix());
        break;

    case GLFW_KEY_U :
        camera->processKeyboard(Camera_Movement::FORWARD, deltaTime);
        rabbitShader->setMat4("view", camera->getViewMatrix());
        break;

    case GLFW_KEY_O :
        camera->processKeyboard(Camera_Movement::BACKWARD, deltaTime);
        rabbitShader->setMat4("view", camera->getViewMatrix());
        break;

    case GLFW_KEY_KP_ADD :
        rabbitModel->processScale(true);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    case GLFW_KEY_MINUS :
        rabbitModel->processScale(false);
        rabbitShader->setMat4("model", rabbitModel->getModelMatrix());
        break;

    default:
        break;
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
    {
        return;
    }

    if (mods == GLFW_KEY_LEFT_CONTROL || mods == GLFW_KEY_RIGHT_CONTROL)
    {
        camera->processMouseMovement(float(xOffset), float(yOffset));
        rabbitShader->setMat4("view", camera->getViewMatrix());
    }
}

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    xOffset = xPos - lastX;
    yOffset = yPos - lastY;
    lastX = xPos;
    lastY = yPos;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera->processMouseScroll(float(yoffset));
    rabbitShader->setMat4("projection", camera->getProjectionMatrix());
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    iniGlfw();

    // glfw 创建窗口
    // ------------------
    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH,  SCR_HEIGHT, "rabbit", nullptr, nullptr);
    if (window == nullptr)
    {
        cout << "创建 GLFW 窗口失败" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
    {
        cout << "无法初始化 GLAD" << endl;
        return -1;
    }

    // 设置全局状态，开启深度检测
    glEnable(GL_DEPTH_TEST);

    // 导入并处理兔子文件
    loadFile(bunnyFile);
    calNormal();

    // 创建着色器对象
    rabbitShader = new Shader(rabbitVertexPath, rabbitFragmentPath);
    lightShader = new Shader(lightVertexPath, lightFragmentPath);

    // 创建摄像机对象
    camera = new Camera();

    // 创建被观察物体的抽象化对象
    rabbitModel = new Model();

    firstMouse = true;

    // 顶点数组、面片数组定义在ply.h文件

    unsigned int rabbitVAO, rabbitVBO, rabbitEBO = 0;
    unsigned int lightVAO, lightVBO, lightEBO = 0;

    confRabbitABB(rabbitVAO, rabbitVBO, rabbitEBO, rabbitVertices, rabbitIndices);
    confLightABB(lightVAO, lightVBO, lightEBO, lightVertices, lightIndices);

    // 绑定回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    rabbitShader->useShaderProgram();

    // 变换矩阵
    rabbitShader->setMat4("projection", camera->getProjectionMatrix());
    rabbitShader->setMat4("view", camera->getViewMatrix());
    rabbitShader->setMat4("model", rabbitModel->getModelMatrix());

    // 材质属性
    rabbitShader->setInt("material.diffuse", 0);
    rabbitShader->setInt("material.specular", 1);

    while (!glfwWindowShouldClose(window))
    {
        updateTime();

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        rabbitShader->useShaderProgram();

        // 光源和观察者位置
        rabbitShader->setVec3("viewPos", camera->getPosition());

        // 材质属性
        rabbitShader->setFloat("material.shininess", 32.0f);

        // 光照属性

        // 平行光
        rabbitShader->setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        rabbitShader->setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        rabbitShader->setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        rabbitShader->setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

        // 点光源 1
        rabbitShader->setVec3("pointLights[0].position", pointLightPositions[0]);
        rabbitShader->setVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        rabbitShader->setVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        rabbitShader->setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        rabbitShader->setFloat("pointLights[0].constant", 1.0f);
        rabbitShader->setFloat("pointLights[0].linear", 0.09f);
        rabbitShader->setFloat("pointLights[0].quadratic", 0.032f);

        // 点光源 2
        rabbitShader->setVec3("pointLights[1].position", pointLightPositions[1]);
        rabbitShader->setVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        rabbitShader->setVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        rabbitShader->setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        rabbitShader->setFloat("pointLights[1].constant", 1.0f);
        rabbitShader->setFloat("pointLights[1].linear", 0.09f);
        rabbitShader->setFloat("pointLights[1].quadratic", 0.032f);

        // 点光源 3
        rabbitShader->setVec3("pointLights[2].position", pointLightPositions[2]);
        rabbitShader->setVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        rabbitShader->setVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        rabbitShader->setVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        rabbitShader->setFloat("pointLights[2].constant", 1.0f);
        rabbitShader->setFloat("pointLights[2].linear", 0.09f);
        rabbitShader->setFloat("pointLights[2].quadratic", 0.032f);

        // 点光源 4
        rabbitShader->setVec3("pointLights[3].position", pointLightPositions[3]);
        rabbitShader->setVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        rabbitShader->setVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        rabbitShader->setVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        rabbitShader->setFloat("pointLights[3].constant", 1.0f);
        rabbitShader->setFloat("pointLights[3].linear", 0.09f);
        rabbitShader->setFloat("pointLights[3].quadratic", 0.032f);

        // 聚光
        rabbitShader->setVec3("spotLight.position", camera->getPosition());
        rabbitShader->setVec3("spotLight.direction", camera->getFront());
        rabbitShader->setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        rabbitShader->setVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        rabbitShader->setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        rabbitShader->setFloat("spotLight.constant", 1.0f);
        rabbitShader->setFloat("spotLight.linear", 0.09f);
        rabbitShader->setFloat("spotLight.quadratic", 0.032f);
        rabbitShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        rabbitShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        glm::vec3 lightColor;
        lightColor.x = float(sin(glfwGetTime() * 2.0f));
        lightColor.y = float(sin(glfwGetTime() * 0.7f));
        lightColor.z = float(sin(glfwGetTime() * 1.3f));

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        glm::vec3 specularColor = glm::vec3(1.0f);

        rabbitShader->setVec3("light.ambient", ambientColor);
        rabbitShader->setVec3("light.diffuse", diffuseColor);
        rabbitShader->setVec3("light.specular", specularColor);

        // 当存在多个顶点数组对象时，需要指定
        glBindVertexArray(rabbitVAO);

        // 指明我们从索引缓冲渲染。使用glDrawElements时，我们会使用当前绑定的索引缓冲对象中的索引进行绘制
        glDrawElements(GL_TRIANGLES, rabbitVertexNum, GL_UNSIGNED_INT, 0);


        lightShader->useShaderProgram();

        glBindVertexArray(lightVAO);

        // 变换矩阵
        lightShader->setMat4("view", camera->getViewMatrix());
        lightShader->setMat4("projection", camera->getProjectionMatrix());

        for (int i = 0; i < 4; i++) {
            lightModel->setTrans(pointLightPositions[i]);
            lightModel->setScale(pointLightScales[i]);
            lightShader->setMat4("model", lightModel->getModelMatrix());
            glDrawElements(GL_TRIANGLES, lightVertexNum, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &rabbitVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &rabbitVBO);
    glDeleteBuffers(1, &rabbitEBO);

    // 释放当前的glfw资源
    glfwTerminate();

    delete rabbitShader;
    delete lightShader;
    delete camera;
    delete rabbitModel;

    rabbitShader = nullptr;
    lightShader = nullptr;
    camera = nullptr;
    rabbitModel = nullptr;

    return a.exec();
}
