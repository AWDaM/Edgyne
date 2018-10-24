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
	root = NewGameObject("root");
	return true;
}

bool ModuleLevel::Start()
{
	App->loader->ReceivedFile("Assets\\BakerHouse\\BakerHouse.FBX");
	return true;
}

GameObject * ModuleLevel::NewGameObject(std::string name, bool with_transform)
{
	GameObject* ret = new GameObject(name);
	if (with_transform)
	{
		ret->AddComponent(TRANSFORM);
	}
	game_objects.push_back(ret);
	return ret;
}

void ModuleLevel::Draw()
{
	std::vector<GameObject*>::iterator item = game_objects.begin();

	while (item != game_objects.end())
	{
		(*item)->Draw();
		item++;
	}

}
