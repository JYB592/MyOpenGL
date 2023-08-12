#pragma once

#include <set>;

class DirLightComponent;
class PointLightComponent;
class SpotLightComponent;
class YYShader;

using namespace std;

class LightManager
{
public:
	static LightManager* GetInstance()
	{
		if (lightManagerInstance == nullptr)
		{
			lightManagerInstance = new LightManager();
		}
		return lightManagerInstance;
	}
	static LightManager* lightManagerInstance;

	void AddDirLight(DirLightComponent* dirLight);
	void RemoveDirLight(DirLightComponent* dirLight);

	void AddPointLight(PointLightComponent* pointLight);
	void RemovePointLight(PointLightComponent* pointLight);

	void AddSpotLight(SpotLightComponent* spotLight);
	void RemoveSpotLight(SpotLightComponent* spotLight);

	void SetLightsShaderData(YYShader* shader);
private:
	set<DirLightComponent*> dirLights = set<DirLightComponent*>();
	set<PointLightComponent*> pointLights = set<PointLightComponent*>();
	set<SpotLightComponent*> spotLights = set<SpotLightComponent*>();
};

