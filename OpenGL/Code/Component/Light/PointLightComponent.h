#pragma once

#include <glm-0.9.9.8/glm/glm.hpp>
#include "YYShader.h"
#include "LightBaseComponent.h"

using namespace glm;

class PointLightComponent : public LightBaseComponent
{
public:
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	PointLightComponent();
	virtual ~PointLightComponent() override;
	virtual void SetShaderData(YYShader* shader, int index);
};



