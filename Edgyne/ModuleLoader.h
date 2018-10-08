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

	bool LoadTexture();



private:
	void LoadVerices(mesh* new_mesh, aiMesh* currentMesh);
	bool LoadTextures(mesh* new_mesh, aiMesh* currentMesh, const aiScene* scene);
	void LoadIndices(mesh* new_mesh, aiMesh* currentMesh);

public:
private:
	struct aiLogStream stream;
};