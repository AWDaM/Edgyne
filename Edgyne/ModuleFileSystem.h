#pragma once

#include "Module.h"
#include "Globals.h"

struct mesh;

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Init(rapidjson::Value& node);

	bool SaveToFile(mesh* mesh);
	bool LoadFromFile();

	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);
	void Load(rapidjson::Document& doc);
};