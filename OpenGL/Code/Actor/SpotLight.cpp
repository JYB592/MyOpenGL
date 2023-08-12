#include "SpotLight.h"
#include "SpotLightComponent.h"
#include "MeshComponent.h"

SpotLight::SpotLight()
{
	AddComponent(new SpotLightComponent());
	MeshComponent* mesh = new MeshComponent("Model/SpotLight/SpotLight.obj");
	Scale(0.1f);
	Material mat = Material();
	mat.lightEffect = false;
	mat.vstr = "ShaderCode/shader.vs";
	mat.fstr = "ShaderCode/lightShader.fs";
	mesh->SetMaterial(mat);
	AddComponent(mesh);
}

SpotLight::~SpotLight()
{
}
