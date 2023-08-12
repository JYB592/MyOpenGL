#include "DirLight.h"
#include <DirLightComponent.h>

DirLight::DirLight()
{
	AddComponent(new DirLightComponent());
}

DirLight::~DirLight()
{
}
