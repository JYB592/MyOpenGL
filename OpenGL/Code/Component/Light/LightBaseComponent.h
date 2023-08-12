#pragma once
#include <glm-0.9.9.8/glm/glm.hpp>

#include "SceneComponent.h"

class YYShader;

using namespace glm;

class LightBaseComponent : public SceneComponent
{
public:
	LightBaseComponent();
	virtual ~LightBaseComponent() override;

	virtual void SetShaderData(YYShader* shader, int index) = 0;
protected:
	vec3 ambient = vec3(0.05f, 0.05f, 0.05f);//������ǿ������ɫ
	vec3 diffuse = vec3(0.8f, 0.8f, 0.8f);//�������ǿ������ɫ
	vec3 specular = vec3(1.0f, 1.0f, 1.0f);//�������ɫ��ǿ��
};

