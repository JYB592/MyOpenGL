#include "Actor.h"
#include "Component.h"
#include <iostream>
#include "SceneComponent.h"

using namespace std;

Actor::Actor()
{
	AddComponent(new SceneComponent());
}

Actor::~Actor()
{

}

void Actor::Translate(float x, float y, float z)
{
	rootCom->Translate(x, y, z);
}

void Actor::Rotate(float angle, float x, float y, float z)
{
	rootCom->Rotate(angle, x, y, z);
}

void Actor::Scale(float s)
{
	rootCom->Scale(s);
}

void Actor::RotateWorld(float angle, float x, float y, float z)
{
	rootCom->RotateWorld(angle, x, y, z);
}

void Actor::AddComponent(Component* com)
{
	com->owner = this;
	shared_ptr<Component> comShare(com);
    components[com->name] = comShare;

	SceneComponent* sceneCom = dynamic_cast<SceneComponent*>(com);
	if (sceneCom != nullptr)
	{
		if (rootCom == nullptr)
		{
			rootCom = sceneCom;
		}
		else
		{
			sceneCom->SetParent(rootCom);
		}
	}
}
