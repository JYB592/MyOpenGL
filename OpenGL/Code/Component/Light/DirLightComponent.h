#pragma once
#include <glm-0.9.9.8/glm/glm.hpp>
#include "YYShader.h"
#include "LightBaseComponent.h"
using namespace glm;

class DirLightComponent : public LightBaseComponent
{
public:
	DirLightComponent();
	virtual ~DirLightComponent() override;
	virtual void SetShaderData(YYShader* shader, int index);
};

