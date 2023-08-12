#pragma once

#include <glm-0.9.9.8/glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

class YYShader;
class Texture;
class YYMesh;
class MeshComponent;

using namespace std;
using namespace glm;

class YYModel
{
	friend class MeshComponent;
public:
	static YYShader* defShader;
	static YYShader* singleColorShader;

	// model data 
	MeshComponent* owner = nullptr;
	vector<Texture> textures_loaded;	// 存储到目前为止加载的所有纹理，优化以确保纹理不会加载超过一次。
	vector<YYMesh*>    meshes; 
	string directory; //modle文件所在路径
	bool gammaCorrection;


	YYModel(string const& path, bool gamma = false);
	virtual ~YYModel();
	void Draw();

	void SetOwnerCom(MeshComponent* pOwner);
	void SetEspecialShader(const char* vstr, const char* fstr);

	//void Translate(float x, float y, float z);
	//void Rotate(float angle, float x, float y, float z);
	//void Scale(float s);
	//void RotateWorld(float angle, float x, float y, float z);

	//glm::vec3 GetPos();

protected:
	YYShader* ourShader = nullptr;

	glm::mat4 modelM = mat4(1);

private:

	void StartStencil();
	void EndStencil();

	const mat4& GetModelM();

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    YYMesh* processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);