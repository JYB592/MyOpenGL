#include "GameInstance.h"
#include "YYCamera.h"
#include "YYModelManager.h"

extern YYCamera* camera;

GameInstance* GameInstance::gameInstance = nullptr;

void GameInstance::Init()
{
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    //开启混合
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    //开启背面剔除
    glEnable(GL_CULL_FACE);
	YYModelManager::GetInstance()->init();
    camera = new YYCamera(glm::vec3(0.0f, 2.0f, 6.0f));
}

void GameInstance::Update()
{
	camera->Update();
	YYModelManager::GetInstance()->Update();//绘制
}

void GameInstance::LateUpdate()
{
	camera->LateUpdate();
}
