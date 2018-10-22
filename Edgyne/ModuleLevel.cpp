#include "Application.h"
#include "ModuleLevel.h"

#include "SDL\include\SDL_opengl.h"

#include "ModuleLoader.h"

ModuleLevel::ModuleLevel(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Level";
}


ModuleLevel::~ModuleLevel()
{
}

bool ModuleLevel::Init(rapidjson::Value& node)
{
	return true;
}

bool ModuleLevel::Start()
{
	App->loader->ReceivedFile("Assets\\BakerHouse\\BakerHouse.FBX");
	return true;
}

void ModuleLevel::Draw()
{


}
