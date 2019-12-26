#include "shader.h"

Shader::Shader(string vertexPath, string fragmentPath) 
{
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;

    // 确保可以正确抛出错误
    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    try 
    {
        vShaderFile.open(vertexPath, ios::in);
        fShaderFile.open(fragmentPath, ios::in);

        stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } 
    catch (ifstream::failure e) 
    {
        cout << "着色器文件读取失败" << endl;
    }

    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();

    // 构建并编译着色器
    // -----------------------
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // 链接着色器
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::useShaderProgram() 
{
    // 激活着色器
    glUseProgram(this->ID);
}

void Shader::setBool(string name, bool value) const 
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::setInt(string name, int value) const 
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(string name, float value) const 
{
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloatVector(string name, float value1, float value2, float value3, float value4) const 
{
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value1, value2, value3, value4);
}

void Shader::setVec3(string name, glm::vec3 pc)
{
    glad_glUniform3f(glGetUniformLocation(this->ID, name.c_str()), pc.x, pc.y, pc.z);
}

void Shader::setMat4(string name, glm::mat4 trans) 
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
}

void Shader::checkCompileErrors(unsigned int shader, string type) 
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") 
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) 
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            cout << type << "着色器编译失败" << endl;
        }
    } 
    else 
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) 
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cout << type << "着色器链接失败" << endl;
        }
    }
}
