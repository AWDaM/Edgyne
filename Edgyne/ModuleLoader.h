#ifndef __MODULE_LOADER_H__
#define __MODULE_LOADER_H__


#include "Module.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"


//class mesh;
void AssimpLoggerLoad(const char* message, char* user);

class GameObject;
class Mesh;
class Material;

class ModuleLoader : public Module
{
public:
	ModuleLoader(Application* app, bool start_enabled = true);
	~ModuleLoader();

	bool Init(rapidjson::Value& node);

	update_status Update(float dt);
	bool CleanUp();
	void Save(rapidjson::Document & doc, rapidjson::FileWriteStream& os);
	void Load(rapidjson::Document& doc);

	bool Import(const std::string& file);

	void LoadTextureFromLibrary(const char* path, Material* material);

	void ReceivedFile(const char* path);

	void SaveScene(std::string name);
	void LoadScene(std::string name);
	void AddGameObjectsFromFile(GameObject* parent, rapidjson::Document& document);
private:
	void SaveMesh(Mesh* mesh);
	std::string SaveMaterial(const std::string& path);
	
	void LoadInfo(GameObject* game_object, aiMesh* currentMesh,aiNode* node);
	void LoadVerices(Mesh* new_mesh, aiMesh* currentMesh);
	void LoadColor(Material* new_mesh, aiMaterial* mat);
	bool LoadTextures(Mesh* new_mesh,Material* material, aiMesh* currentMesh, const aiScene* scene, const std::string& file);
	void LoadNormals(Mesh* new_mesh, aiMesh* currentMesh);
	void LoadIndices(Mesh* new_mesh, aiMesh* currentMesh);
	void LoadBoundingBox(Mesh* new_mesh, aiMesh* currentMesh);

	void LoadMeshesFromFile(Mesh* _mesh);

	void LoadAllNodesMeshes(aiNode* node, const aiScene* scene, const std::string& file, GameObject* parent);
	bool CheckTexturePaths(std::string file, std::string texPath, std::string& texActualPath);

public:
	//char* assimpLog;
	//char* user;
private:
	struct aiLogStream stream;
};

#endif // !__MODULE_LOADER_H__