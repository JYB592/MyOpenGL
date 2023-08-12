#pragma once

#include <glm-0.9.9.8/glm/ext/matrix_float4x4.hpp>

#include "Component.h"
#include <set>

using namespace glm;

class SceneComponent : public Component
{
public:
	SceneComponent();
	virtual ~SceneComponent() override;

	SceneComponent* parent = nullptr;
	set<SceneComponent*> children;

	const mat4& GetModelM() const;

	bool IsMyParent(SceneComponent* sceneCom);
	void SetParent(SceneComponent* sceneCom);
	const mat4& GetWorldMat() const;
	const vec3& GetWorldPosition() const;

	void Translate(float x, float y, float z);
	void Rotate(float angle, float x, float y, float z);
	float GetScale() const;
	void Scale(float s);
	void RotateWorld(float angle, float x, float y, float z);

private:
	mat4 modelM = mat4(1.0f);
};

