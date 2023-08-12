#pragma once

//#include <glad/glad.h>; // ����glad����ȡ���еı���OpenGLͷ�ļ�

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
    // ����ID
    unsigned int ID;

    // ��������ȡ��������ɫ��
    YYShader(const char* vertexPath, const char* fragmentPath);
    ~YYShader();
    // ʹ��/�������
    void Use();
    // uniform���ߺ���
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVector3(const std::string& name, float v1, float v2, float v4) const;
    void SetVector4(const std::string& name, float v1, float v2, float v3, float v4) const;
    void SetMatrix4(const std::string& name, glm::mat4 mat) const;
};

