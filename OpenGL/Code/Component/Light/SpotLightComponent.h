#pragma once

#include <glm-0.9.9.8/glm/glm.hpp>
#include "YYShader.h"
#include "LightBaseComponent.h"

using namespace glm;

class SpotLightComponent : public LightBaseComponent
{
public:
	float constant = 1.0f;
	float linear = 0.0009f;
	float quadratic = 0.00032f;
    float cutOff = cos(radians(30.5f));
    float outerCutOff = glm::cos(glm::radians(35.0f));

	SpotLightComponent();
	virtual ~SpotLightComponent() override;
	virtual void SetShaderData(YYShader* shader, int index);
};

