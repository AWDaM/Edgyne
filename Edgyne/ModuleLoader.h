#pragma once
#include "Module.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"


class ModuleLoader : public Module
{
public:
	ModuleLoader(Application* app, bool start_enabled = true);
	~ModuleLoader();

	bool Init(rapidjson::Document& document);

	update_status Update(float dt);
	bool CleanUp();

	bool Import(const std::string& file);


private:
public:
private:
	struct aiLogStream stream;
};

