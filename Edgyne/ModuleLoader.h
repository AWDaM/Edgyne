#pragma once
#include "Module.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "ModuleRenderer3D.h"

//class mesh;
void AssimpLoggerLoad(const char* message, char* user);


class ModuleLoader : public Module
{
public:
	ModuleLoader(Application* app, bool start_enabled = true);
	~ModuleLoader();

	bool Init(rapidjson::Value& node);


	update_status Update(float dt);
	bool CleanUp();
	void Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os);
	void Load(rapidjson::Document& doc);

	bool Import(const std::string& file);

	bool ImportTexture(const char* path);

	void ReceivedFile(const char* path);

private:
	void LoadInfo(mesh* new_mesh, aiMesh* currentMesh,aiNode* node);
	void LoadVerices(mesh* new_mesh, aiMesh* currentMesh);
	void LoadColor(mesh* new_mesh, aiMaterial* mat);
	bool LoadTextures(mesh* new_mesh, aiMesh* currentMesh, const aiScene* scene, const std::string& file);
	void LoadNormals(mesh* new_mesh, aiMesh* currentMesh);
	void LoadIndices(mesh* new_mesh, aiMesh* currentMesh);
	void LoadBoundingBox(mesh* new_mesh, aiMesh* currentMesh);

	void LoadMeshesFromFile(mesh* _mesh);

	void LoadAllNodesMeshes(aiNode* node, const aiScene* scene, const std::string& file);
	bool CheckTexturePaths(std::string file, aiString texPath);
public:
	//char* assimpLog;
	//char* user;
private:
	struct aiLogStream stream;
	std::string MeshPath;
	std::string TexturePath;

};