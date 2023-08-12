#pragma once
#include "SceneComponent.h"

class YYModel;

//ģ����ͼ
struct Material {
	float alpha = 1;
	float shininess = 100.0f;//�����
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

