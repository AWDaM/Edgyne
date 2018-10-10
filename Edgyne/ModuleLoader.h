#pragma once
#include "Module.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "ModuleRenderer3D.h"

//class mesh;

class ModuleLoader : public Module
{
public:
	ModuleLoader(Application* app, bool start_enabled = true);
	~ModuleLoader();

	bool Init(rapidjson::Document& document);

	update_status Update(float dt);
	bool CleanUp();

	bool Import(const std::string& file);

	bool ImportTexture(const char* path);

	void ReceivedFile(const char* path);

private:
	void LoadVerices(mesh* new_mesh, aiMesh* currentMesh);
	void LoadColor(mesh* new_mesh, aiMaterial* mat);
	bool LoadTextures(mesh* new_mesh, aiMesh* currentMesh, const aiScene* scene, const std::string& file);
	void LoadNormals(mesh* new_mesh, aiMesh* currentMesh);
	void LoadIndices(mesh* new_mesh, aiMesh* currentMesh);
	void LoadBoundingBox(mesh* new_mesh, aiMesh* currentMesh);

	bool CheckTexturePaths(std::string file, aiString texPath);
public:

private:
	struct aiLogStream stream;

};