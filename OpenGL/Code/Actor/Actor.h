#pragma once
#include <memory>
#include <map>
#include <string>

using namespace std;

class Component;
class SceneComponent;

class Actor
{
public:
	map<string, shared_ptr<Component>> components;
	SceneComponent* rootCom;

	Actor();
	virtual ~Actor();
	void AddComponent(Component* com);

	void Translate(float x, float y, float z);
	void Rotate(float angle, float x, float y, float z);
	void Scale(float s);
	void RotateWorld(float angle, float x, float y, float z);
private:

};

