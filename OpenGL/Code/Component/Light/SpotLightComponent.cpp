#include "SpotLightComponent.h"

#include "YYShader.h"
#include "LightManager.h"

SpotLightComponent::SpotLightComponent()
{
    name = "SpotLightComponent";

    ambient = vec3(0.0f, 0.0f, 0.0f);
    diffuse = vec3(1.0f, 1.0f, 1.0f);
    specular = vec3(1.0f, 1.0f, 1.0f);

    LightManager::GetInstance()->AddSpotLight(this);
}

SpotLightComponent::~SpotLightComponent()
{
    LightManager::GetInstance()->RemoveSpotLight(this);
}

void SpotLightComponent::SetShaderData(YYShader* shader, int index)
{
    vec3 pos = GetWorldPosition();
    shader->SetVector3("spotLights[" + to_string(index) + "].position", pos.x, pos.y, pos.z);
    shader->SetVector3("spotLights[" + to_string(index) + "].direction", 0.0f, 0.0f, 1.0f);
    shader->SetFloat("spotLights[" + to_string(index) + "].cutOff", cutOff);
    shader->SetFloat("spotLights[" + to_string(index) + "].outerCutOff", outerCutOff);

    shader->SetFloat("spotLights[" + to_string(index) + "].constant", constant);
    shader->SetFloat("spotLights[" + to_string(index) + "].linear", linear);
    shader->SetFloat("spotLights[" + to_string(index) + "].quadratic", quadratic);

    shader->SetVector3("spotLights[" + to_string(index) + "].ambient", ambient.x, ambient.y, ambient.z);
    shader->SetVector3("spotLights[" + to_string(index) + "].diffuse", diffuse.x, diffuse.y, diffuse.z);
    shader->SetVector3("spotLights[" + to_string(index) + "].specular", specular.x, specular.y, specular.z);
}