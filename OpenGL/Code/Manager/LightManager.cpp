#include "LightManager.h"

#include <GLFW/glfw3.h>
#include "DirLightComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "YYShader.h"

LightManager*  LightManager::lightManagerInstance = nullptr;

void LightManager::AddDirLight(DirLightComponent* dirLight)
{
	dirLights.insert(dirLight);
}

void LightManager::RemoveDirLight(DirLightComponent* dirLight)
{
	dirLights.erase(dirLight);
}

void LightManager::AddPointLight(PointLightComponent* pointLight)
{
	pointLights.insert(pointLight);
}

void LightManager::RemovePointLight(PointLightComponent* pointLight)
{
	pointLights.erase(pointLight);
}

void LightManager::AddSpotLight(SpotLightComponent* spotLight)
{
	spotLights.insert(spotLight);
}

void LightManager::RemoveSpotLight(SpotLightComponent* spotLight)
{
	spotLights.erase(spotLight);
}

void LightManager::SetLightsShaderData(YYShader* shader)
{
	int dirLightNum = 0;
	for (auto light : dirLights)
	{
		light->SetShaderData(shader, dirLightNum);
		dirLightNum += 1;
	}
	shader->SetInt("dirLightNum", dirLightNum);

	int pointLightNum = 0;
	for (auto light : pointLights)
	{
		light->SetShaderData(shader, pointLightNum);
		pointLightNum += 1;
	}
	shader->SetInt("pointLightNum", pointLightNum);

	int spotLightNum = 0;
	for (auto light : spotLights)
	{
		light->SetShaderData(shader, spotLightNum);
		spotLightNum += 1;
	}
	shader->SetInt("spotLightNum", spotLightNum);
}
