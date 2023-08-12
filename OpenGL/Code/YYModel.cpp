#include "YYModel.h"

#include <glad/glad.h>
#include <iostream>
#include <glm-0.9.9.8/glm/glm.hpp>
#include <glm-0.9.9.8/glm/gtc/matrix_transform.hpp>
#include <glm-0.9.9.8/glm/gtc/type_ptr.hpp>
#include <memory>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "YYMesh.h"
#include "YYCamera.h"
#include "YYShader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "YYModelManager.h"
#include "LightManager.h"
#include "MeshComponent.h"

using namespace std;

extern YYCamera* camera;
extern  int SCR_WIDTH;
extern  int SCR_HEIGHT;

YYShader*  YYModel::defShader = nullptr;
YYShader* YYModel::singleColorShader = nullptr;

YYModel::YYModel(string const& path, bool gamma) : 
    gammaCorrection(gamma)
{
    loadModel(path);
}
YYModel::~YYModel()
{
    if (ourShader)
    {
        delete ourShader;
    }
    for (YYMesh* mesh : meshes)
    {
        delete mesh;
    }
    meshes.clear();
}

void YYModel::SetOwnerCom(MeshComponent* pOwner)
{
    owner = pOwner;
}

void YYModel::SetEspecialShader(const char* vstr, const char* fstr)
{
    ourShader = new YYShader(vstr, fstr);
}

void YYModel::Draw()
{
    //////////////////////////////////////////////////设置shader
    YYShader* shader = ourShader ? ourShader : defShader;
    shader->Use();
    if (!owner || owner->material.lightEffect)
    {
        LightManager::GetInstance()->SetLightsShaderData(shader);
    }

    StartStencil();

    //M模型变换
    shader->SetMatrix4("modelM", GetModelM());
    //V视口变换
    shader->SetMatrix4("viewM", camera->viewM);
    //P投影变换
    shader->SetMatrix4("projectionM", camera->projectionM);
    
    const glm::vec3& cameraPos = camera->GetPosition();
    shader->SetVector3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);//摄像机位置

    //shader->SetVector3("material.specular", 1.0f, 1.0f, 1.0f);
    shader->SetFloat("material.shininess", owner ? owner->material.shininess : 100.0f);
    shader->SetFloat("material.alpha", owner ? owner->material.alpha : 1.0f);
    
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i]->Draw(shader);
    }

    EndStencil();
}

void YYModel::StartStencil()
{
    owner->StartStencil();
}

void YYModel::EndStencil()
{
    owner->EndStencil();
}

//void YYModel::Translate(float x, float y, float z)
//{
//    modelM = glm::translate(modelM, glm::vec3(x, y, z));
//}
//
//void YYModel::Rotate(float angle, float x, float y, float z)
//{
//    modelM = glm::rotate(modelM, glm::radians(angle), glm::vec3(x, y, z)); // 构造旋转矩阵
//}
//
//void YYModel::Scale(float s)
//{
//    modelM = glm::scale(modelM, glm::vec3(s, s, s));
//}
//
//void YYModel::RotateWorld(float angle, float x, float y, float z)
//{
//    glm::mat4 m = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z)); // 构造旋转矩阵
//    modelM = m * modelM;
//}

//glm::vec3 YYModel::GetPos()
//{
//    return glm::vec3(modelM[3]);  // 返回矩阵第4列向量的xyz分量
//}

const mat4& YYModel::GetModelM()
{
    if (owner != nullptr)
    {
        return owner->GetWorldMat();
    }
    return modelM;
}

void YYModel::loadModel(string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    // 检索文件路径的目录路径
    directory = path.substr(0, path.find_last_of('/'));

    // 递归地处理ASSIMP的根节点
    processNode(scene->mRootNode, scene);
}

void YYModel::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

YYMesh* YYModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // 遍历每个网格的顶点
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // 纹理坐标
        if (mesh->mTextureCoords[0]) // 网格是否包含纹理坐标?
        {
            glm::vec2 vec;
            //一个顶点最多可以包含8个不同的纹理坐标。因此，我们假设我们不会使用顶点可以有多个纹理坐标的模型，所以我们总是取第一个集合(0)。
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // 现在遍历每个网格的面(面是一个网格的三角形)并检索相应的顶点索引。
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // 检索面的所有索引并将它们存储在索引向量中
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return new YYMesh(vertices, indices, textures);
}

vector<Texture> YYModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // 检查之前是否加载了纹理，如果是，继续下一个迭代:跳过加载新纹理
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
