#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm-0.9.9.8/glm/glm.hpp>
#include <glm-0.9.9.8/glm/gtc/matrix_transform.hpp>
#include <glm-0.9.9.8/glm/gtc/type_ptr.hpp>

#include "YYShader.h"
#include "YYCamera.h"
#include "YYWindow.h"
#include "YYModelManager.h"
#include "YYModel.h"
#include "DirLight.h"
#include "LightManager.h"
#include "MeshComponent.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <stb_image.h>
#include "GameInstance.h";

using namespace std;
using namespace glm;

// settings
int SCR_WIDTH = 1000;
int SCR_HEIGHT = 600;

// camera
YYCamera* camera = nullptr;

// update
float deltaTime = 0; // 当前帧与上一帧的时间差
float fpsDeltaTime = 1.0f / 90.0f;
float fpsDeltaTimeLast= 0;
float openGameTime = 0;//游戏开启时的时间
float gameTime = 0; //游戏运行时长

int main(void)
{
    GLFWwindow* YYWindow = YYWindow::createWindow();
    GameInstance* gameInstance = GameInstance::GetInstance();

    gameInstance->Init();

    ///////////////////////////Actor
    Actor* model1 = new Actor();
    MeshComponent* meshcom = new MeshComponent("Model/nanosuit/nanosuit.obj");
    model1->AddComponent(meshcom);
    model1->Scale(0.2f);
    Material material = Material();
    material.alpha = 0.5f;
    meshcom->SetMaterial(material);
    Actor* model2 = new Actor();
    model2->AddComponent(new MeshComponent("Model/nanosuit/nanosuit.obj"));
    model2->Scale(0.2f);
    model2->Translate(3, 0, 0);
    Actor* model3 = new Actor();
    model3->AddComponent(new MeshComponent("Model/nanosuit/nanosuit.obj"));
    model3->Scale(0.2f);
    model3->Translate(-3, 0, 0);
    //光
    DirLight* dirLight = new DirLight();
    PointLight* pointLight1 = new PointLight();
    pointLight1->Translate(0, 2, -2);
    SpotLight* spotLight1 = new SpotLight();
    spotLight1->Translate(-3, 0, -2);
    SpotLight* spotLight2 = new SpotLight();
    spotLight2->Translate(0, 1.5, -2);
    SpotLight* spotLight3 = new SpotLight();
    spotLight3->Translate(3, 3, -2);
    ///////////////////////////////

    //////////////////////////////帧缓冲
    int lessenDraw = 1; //设置渲染分辨率
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // 纹理颜色附件
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH / lessenDraw, SCR_HEIGHT / lessenDraw, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
    // 渲染缓冲的深度和模板附件
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH / lessenDraw, SCR_HEIGHT / lessenDraw);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //顶点缓冲设置
    float quadVertices[] = //四边形
    {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW); //传入的顶点数据
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    YYShader* framebufferShader = new YYShader("ShaderCode/FrameBuffer.vs", "ShaderCode/FrameBuffer.fs");
    //////////////////////////////////////////

    //////////////////////////////////////////////////渲染循环
    openGameTime = (float)glfwGetTime();
    bool isdelete = false;
    while (!glfwWindowShouldClose(YYWindow))
    {
        float currentFrame = (float)glfwGetTime();
        gameTime = currentFrame - openGameTime;
        //cout << "time: " << gameTime << endl;
        //if (gameTime > 5.0f && !isdelete) {
        //    isdelete = true;
        //    delete model1;
        //}
        float dtime = currentFrame - fpsDeltaTimeLast;
        if (dtime < fpsDeltaTime) { continue; }
        deltaTime = currentFrame - fpsDeltaTimeLast;
        fpsDeltaTimeLast = currentFrame;
        //cout << "FPS: " << ceil(1.0f / deltaTime) << endl;

        YYWindow::processInput();//输入事件
        pointLight1->RotateWorld(1, 0, 1, 0);

        //渲染
        glViewport(0, 0, SCR_WIDTH / lessenDraw, SCR_HEIGHT / lessenDraw);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);//切换帧缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //glClear时将颜色缓冲区的颜色全部换成特定值
        glStencilMask(0xFF); //深度缓冲写入时对值的调整
        glClearStencil(0); //glClear时对所有模板缓冲区中的值进行写入0
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//清除颜色，深度，模板缓冲
        gameInstance->Update();
        gameInstance->LateUpdate();
        //std::cout << glGetError() << std::endl; // 返回 0 (无错误)
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        //后处理
        glBindFramebuffer(GL_FRAMEBUFFER, 0);//切换回默认帧缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //glClear时将颜色缓冲区的颜色全部换成特定值
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//清除颜色，深度，模板缓冲
        framebufferShader->Use();
        framebufferShader->SetInt("screenTexture", 0);
        framebufferShader->SetFloat("width", SCR_WIDTH);
        framebufferShader->SetFloat("height", SCR_HEIGHT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

     
        //交换颜色缓冲，将窗口像素值画到屏幕上
        glfwSwapBuffers(YYWindow);
    }

    //释放资源
    glfwTerminate();

    return 0;
}