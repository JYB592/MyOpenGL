#include "MeshComponent.h"
#include "YYModel.h"
#include "YYModelManager.h"
#include <glad/glad.h>
#include "YYShader.h"
#include "YYCamera.h"
#include "YYMesh.h"

extern YYCamera* camera;
extern  int SCR_WIDTH;
extern  int SCR_HEIGHT;

MeshComponent::MeshComponent(string const& path)
{
	name = "MeshComponent";

	YYModel* mm = YYModelManager::GetInstance()->AddModel(path);
	mm->SetOwnerCom(this);
	model = mm;
}

MeshComponent::~MeshComponent()
{
	YYModelManager::GetInstance()->RemoveModel(model);
}

void MeshComponent::SetMaterial(const Material& PMaterial)
{
	material = PMaterial;

	if (material.vstr != nullptr && material.fstr != nullptr)
	{
		model->SetEspecialShader(material.vstr, material.fstr);
	}
}

void MeshComponent::StartStencil()
{
	//ģ�����ExampleExampleExampleExampleExampleExampleExample
	//����ģ�����
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//
	glStencilMask(0xff); // ģ�建��д��ʱ���޸�
	glStencilFunc(GL_ALWAYS, 1, 0xff);
}

void MeshComponent::EndStencil()
{
    //ģ�����ExampleExampleExampleExampleExampleExampleExample
    YYShader* shader = YYModel::singleColorShader;
    shader->Use();
    shader->SetVector3("color", 1, 0, 0);
    //Mģ�ͱ任
    shader->SetMatrix4("modelM", GetWorldMat());
    //V�ӿڱ任
    glm::mat4 viewM = camera->GetViewMatrix();
    shader->SetMatrix4("viewM", viewM);
    //PͶӰ�任
    glm::mat4 projectionM = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader->SetMatrix4("projectionM", projectionM);

    shader->SetVector3("color", 1, 0, 0);
    //float thescale = owner->GetScale();
    //owner->Scale(thescale * 1.1f);
    glStencilMask(0xff); // ÿһλ��д��ģ�建��ʱ������0������д�룩
    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    //glDisable(GL_DEPTH_TEST);
    for (unsigned int i = 0; i < model->meshes.size(); i++)
    {
        glBindVertexArray(model->meshes[i]->VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(model->meshes[i]->indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    //owner->Scale(thescale);
    //glEnable(GL_DEPTH_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glDisable(GL_STENCIL_TEST);
}
