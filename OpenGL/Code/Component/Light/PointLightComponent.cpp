#include "PointLightComponent.h"

#include "YYShader.h"
#include "LightManager.h"

PointLightComponent::PointLightComponent()
{
    name = "PointLightComponent";

    ambient = vec3(0.05f, 0.05f, 0.05f);
    diffuse = vec3(0.8f, 0.8f, 0.8f);
    specular = vec3(1.0f, 1.0f, 1.0f);

    LightManager::GetInstance()->AddPointLight(this);
}

PointLightComponent::~PointLightComponent()
{
    LightManager::GetInstance()->RemovePointLight(this);
}

void PointLightComponent::SetShaderData(YYShader* shader, int index)
{
    vec3 pos = GetWorldPosition();
    shader->SetVector3("pointLights[" + to_string(index) + "].position", pos.x, pos.y, pos.z);

    shader->SetFloat("pointLights[" + to_string(index) + "].constant", constant);
    shader->SetFloat("pointLights[" + to_string(index) + "].linear", linear);
    shader->SetFloat("pointLights[" + to_string(index) + "].quadratic", quadratic);

    shader->SetVector3("pointLights[" + to_string(index) + "].ambient", ambient.x, ambient.y, ambient.z);
    shader->SetVector3("pointLights[" + to_string(index) + "].diffuse", diffuse.x, diffuse.y, diffuse.z);
    shader->SetVector3("pointLights[" + to_string(index) + "].specular", specular.x, specular.y, specular.z);
}
