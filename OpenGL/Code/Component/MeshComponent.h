#pragma once
#include "SceneComponent.h"

class YYModel;

//模型贴图
struct Material {
	float alpha = 1;
	float shininess = 100.0f;//反光度
	bool lightEffect = true;
	const char* vstr = nullptr;
	const char* fstr = nullptr;
};

class MeshComponent : public SceneComponent
{
public:
	MeshComponent(string const& path);
	virtual ~MeshComponent() override;

	Material material = Material();

	void SetMaterial(const Material& PMaterial);
	virtual void StartStencil();
	virtual void EndStencil();
private:
	YYModel* model;
};

