#include "Application.h"
#include "ModuleLevel.h"
#include "GameObject.h"

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
	root = NewGameObject();
	return true;
}

bool ModuleLevel::Start()
{
	App->loader->ReceivedFile("Assets\\BakerHouse\\BakerHouse.FBX");
	return true;
}

GameObject * ModuleLevel::NewGameObject(bool with_transform)
{
	GameObject* ret = new GameObject();
	if (with_transform)
	{
		ret->AddComponent(TRANSFORM);
	}

	return ret;
}

void ModuleLevel::Draw()
{


}
