#pragma once
#include "Module.h"
#include "QuadTreeChechu.h"

#include <vector>

class GameObject;

class ModuleLevel :
	public Module
{
public:
	ModuleLevel(Application* app, bool start_enabled = true);
	~ModuleLevel();
	bool Init(rapidjson::Value& node);
	bool Start();

	GameObject* NewGameObject(std::string name, bool with_transform = true);
	
	void Draw();
private:
public:
	QuadTreeChechu quadTree;
	std::vector<GameObject*> game_objects;
	GameObject* root = nullptr;
	GameObject* selected_game_object = nullptr;
};

