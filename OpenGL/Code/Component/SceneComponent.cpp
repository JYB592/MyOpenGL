#include "SceneComponent.h"
#include <glm-0.9.9.8/glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

SceneComponent::SceneComponent()
{
	name = "SceneComponent";
}

SceneComponent::~SceneComponent()
{
}

const mat4& SceneComponent::GetModelM() const
{
    return modelM;
}

bool SceneComponent::IsMyParent(SceneComponent* sceneCom)
{
    SceneComponent* aa = nullptr;
    if (parent == nullptr) { return false; }
    SceneComponent* parentVar = parent;
    while (parentVar)
    {
        if (parentVar == sceneCom)
        {
            return true;
        }
        parentVar = parentVar->parent;
    }
    return false;
}

void SceneComponent::SetParent(SceneComponent* sceneCom)
{
    if (this == sceneCom) { return; }
    if (sceneCom->IsMyParent(this)) { return ; }
    if (parent != nullptr)
    {
        parent->children.erase(this);
    }
    parent = sceneCom;
    parent->children.insert(this);
}

const mat4& SceneComponent::GetWorldMat() const
{
    mat4 rtn = modelM;
    SceneComponent* parentVar = parent;
    while (parentVar != nullptr)
    {
        rtn = parent->modelM * rtn;
        parentVar = parent->parent;
    }

    return rtn;
}

const vec3& SceneComponent::GetWorldPosition() const
{
    mat4 rtn = modelM;
    SceneComponent* parentVar = parent;
    while (parentVar != nullptr)
    {
        rtn = parentVar->modelM * rtn;
        parentVar = parentVar->parent;
    }

    return vec3(rtn[3]);  // 返回矩阵第4列向量的xyz分量
}

void SceneComponent::Translate(float x, float y, float z)
{
    mat4 m = translate(mat4(1.0f), vec3(x, y, z));
    modelM = m * modelM;
}

void SceneComponent::Rotate(float angle, float x, float y, float z)
{
    modelM = rotate(modelM, radians(angle), vec3(x, y, z)); // 构造旋转矩阵
}

float SceneComponent::GetScale() const
{
    glm::mat3 rotation_matrix = glm::mat3(modelM);
    return glm::length(rotation_matrix[0]);
}

void SceneComponent::Scale(float s)
{
    float nows = glm::length(glm::mat3(modelM)[0]);
    float ss = s / nows;
    mat4 m = glm::scale(mat4(1.0f), vec3(ss, ss, ss));
    modelM = modelM * m;
}

void SceneComponent::RotateWorld(float angle, float x, float y, float z)
{
    mat4 m = rotate(mat4(1.0f), radians(angle), vec3(x, y, z)); // 构造旋转矩阵
    modelM = m * modelM;
}


