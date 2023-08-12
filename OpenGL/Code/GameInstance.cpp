#include "GameInstance.h"
#include "YYCamera.h"
#include "YYModelManager.h"

extern YYCamera* camera;

GameInstance* GameInstance::gameInstance = nullptr;

void GameInstance::Init()
{
    //������Ȳ���
    glEnable(GL_DEPTH_TEST);
    //�������
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    //���������޳�
    glEnable(GL_CULL_FACE);
	YYModelManager::GetInstance()->init();
    camera = new YYCamera(glm::vec3(0.0f, 2.0f, 6.0f));
}

void GameInstance::Update()
{
	camera->Update();
	YYModelManager::GetInstance()->Update();//����
}

void GameInstance::LateUpdate()
{
	camera->LateUpdate();
}
