#include "PointLight.h"
#include "PointLightComponent.h"
#include "MeshComponent.h"

PointLight::PointLight()
{
	AddComponent(new PointLightComponent());
	MeshComponent* mesh = new MeshComponent("Model/PointLight/PointLight.obj");
	Scale(0.1f);
	Material mat = Material();
	mat.lightEffect = false;
	mat.vstr = "ShaderCode/shader.vs";
	mat.fstr = "ShaderCode/lightShader.fs";
	mesh->SetMaterial(mat);
	AddComponent(mesh);
}

PointLight::~PointLight()
{
}
