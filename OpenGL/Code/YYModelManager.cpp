#include "YYModelManager.h"
#include "YYModel.h"
#include "YYShader.h"
#include "YYCamera.h"
#include <map>
#include "MeshComponent.h"

extern YYCamera* camera;

YYModelManager* YYModelManager::modelManager = new YYModelManager();

YYModelManager::~YYModelManager()
{
	
}

void YYModelManager::init()
{
	YYModel::defShader = new YYShader("ShaderCode/shader.vs", "ShaderCode/shader.fs");
	YYModel::singleColorShader = new YYShader("ShaderCode/singleColor.vs", "ShaderCode/singleColor.fs");
}

YYModel* YYModelManager::AddModel(string const& path, bool gamma)
{
	YYModel* mm = new YYModel(path, gamma);
	Models.insert(mm);
	return mm;
}

void YYModelManager::RemoveModel(YYModel* model)
{
	Models.erase(model);
	delete model;
}

void YYModelManager::Update()
{
	Draw();
}

void YYModelManager::Draw()
{
	vec3 cameraPos = camera->GetPosition();
	multimap<float, YYModel*> mapmodel = multimap<float, YYModel*>();
	for (YYModel* vv : Models)
	{
		glm::vec3 v1 = vv->owner->GetWorldPosition();
		glm::vec3 v2 = cameraPos;
		float lenght = glm::length(v1 - v2);
		mapmodel.insert({ lenght, vv });
	}
	for (std::multimap<float, YYModel*>::reverse_iterator it = mapmodel.rbegin(); it != mapmodel.rend(); ++it)
	{
		it->second->Draw();
	}
}
