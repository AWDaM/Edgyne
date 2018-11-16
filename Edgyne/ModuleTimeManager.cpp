#include "ModuleTimeManager.h"



ModuleTimeManager::ModuleTimeManager(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "TimeManager";
}


ModuleTimeManager::~ModuleTimeManager()
{
}

bool ModuleTimeManager::Init(rapidjson::Value & node)
{
	return true;
}

bool ModuleTimeManager::CleanUp()
{
	return false;
}

void ModuleTimeManager::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
}

void ModuleTimeManager::Load(rapidjson::Document & doc)
{
}
