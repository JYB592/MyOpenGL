#pragma once

//#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm-0.9.9.8/glm/glm.hpp>
#include <glm-0.9.9.8/glm/gtc/matrix_transform.hpp>
#include <glm-0.9.9.8/glm/gtc/type_ptr.hpp>

class YYShader
{
public:
    // 程序ID
    unsigned int ID;

    // 构造器读取并构建着色器
    YYShader(const char* vertexPath, const char* fragmentPath);
    ~YYShader();
    // 使用/激活程序
    void Use();
    // uniform工具函数
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVector3(const std::string& name, float v1, float v2, float v4) const;
    void SetVector4(const std::string& name, float v1, float v2, float v3, float v4) const;
    void SetMatrix4(const std::string& name, glm::mat4 mat) const;
};

