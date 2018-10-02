#include "Application.h"
#include "ModuleLevel.h"

#include "SDL\include\SDL_opengl.h"

#include "ModuleLoader.h"

ModuleLevel::ModuleLevel(Application* app, bool start_enabled) : Module(start_enabled)
{
}


ModuleLevel::~ModuleLevel()
{
}

bool ModuleLevel::Start()
{
	App->loader->Import("Library/warrior.FBX");
	return true;
}

void ModuleLevel::Draw()
{


}
