#pragma once
#include "Module.h"


class ModuleLoader : public Module
{
public:
	ModuleLoader(Application* app, bool start_enabled = true);
	~ModuleLoader();

	bool Init(rapidjson::Document& document);

	bool CleanUp();

	bool Import(const std::string& file);

private:
public:
private:
	struct aiLogStream stream;
};

