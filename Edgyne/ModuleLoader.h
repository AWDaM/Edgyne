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
class ResourceMaterial;
class ResourceMesh;

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

	bool Import(std::string& file);

	void RecursiveGenerateListFromTree(std::list<GameObject*>& buffer, GameObject* root_game_object);
	void LoadTextureFromLibrary(const char* path, ResourceMaterial* material);

	void ReceivedFile(const char* path);

	void SaveObject(std::string name, bool is_scene, std::list<GameObject*> to_save);
	void LoadObject(std::string name, bool is_scene);
	void AddGameObjectsFromFile(GameObject* parent, rapidjson::Document& document);

	bool CheckIfNameExists(const std::string name);
private:
	void SaveMesh(ResourceMesh* mesh);
	std::string SaveMaterial(const std::string& path);
	
	void LoadInfo(GameObject* game_object,aiNode* node);
	void LoadVerices(ResourceMesh* new_mesh, aiMesh* currentMesh);
	void LoadColor(ResourceMesh* new_mesh, aiMaterial* mat);
	bool LoadTextures(ResourceMesh* new_mesh, ResourceMaterial* material, aiMesh* currentMesh, const aiScene* scene, const std::string& file);
	void LoadNormals(ResourceMesh* new_mesh, aiMesh* currentMesh);
	void LoadIndices(ResourceMesh* new_mesh, aiMesh* currentMesh);
	void LoadBoundingBox(ResourceMesh* new_mesh, aiMesh* currentMesh);

	void LoadMeshesFromFile(Mesh* _mesh);

	void LoadAllNodesMeshes(aiNode* node, const aiScene* scene, std::string& file, GameObject* parent);
	bool CheckTexturePaths(std::string file, std::string texPath, std::string& texActualPath);

public:
	//char* assimpLog;
	//char* user;
private:
	struct aiLogStream stream;
};

#endif // !__MODULE_LOADER_H__