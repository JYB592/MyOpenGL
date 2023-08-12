#include "DirLightComponent.h"
#include "YYShader.h"
#include "LightManager.h"

DirLightComponent::DirLightComponent()
{
    name = "DirLightComponent";
    std::cout << "niubi1111" << endl;
    LightManager::GetInstance()->AddDirLight(this);
}

DirLightComponent::~DirLightComponent()
{
    std::cout << "niubi2222" << endl;
    LightManager::GetInstance()->RemoveDirLight(this);
}

void DirLightComponent::SetShaderData(YYShader* shader, int index)
{
    shader->SetVector3("dirLights[" + to_string(index) + "].direction", -0.2f, -1.0f, -0.3f);
    shader->SetVector3("dirLights[" + to_string(index) + "].ambient", ambient.x, ambient.y, ambient.z);
    shader->SetVector3("dirLights[" + to_string(index) + "].diffuse", diffuse.x, diffuse.y, diffuse.z);
    shader->SetVector3("dirLights[" + to_string(index) + "].specular", specular.x, specular.y, specular.z);
}
