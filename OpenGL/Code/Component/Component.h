#pragma once

#include <memory>
#include <string>
#include <glm-0.9.9.8/glm/ext/matrix_float4x4.hpp>

using namespace std;
using namespace glm;

class Actor;

class Component
{
public:
	string name;
	Actor* owner = nullptr;

	Component();
	virtual ~Component();
};

