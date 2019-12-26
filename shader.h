#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "gl.h"

using namespace std;

class Shader 
{
public:
    unsigned int ID;

    Shader(string vertexPath, string fragmentPath);

    void useShaderProgram();

    // uniform工具函数
    void setBool(string name, bool value) const;  
    void setInt(string name, int value) const;   
    void setFloat(string name, float value) const;
    void setFloatVector(string name, float value1, float value2, float value3, float value4) const;
    void setVec3(string name, glm::vec3 pc);
    void setMat4(string name, glm::mat4 trans);

private:
    void checkCompileErrors(unsigned int shader, string type);
};

#endif // SHADER_H
