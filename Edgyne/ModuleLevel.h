#pragma once
#include "Module.h"

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

	GameObject* NewGameObject(bool with_transform = true);
	void Draw();
private:
public:
	std::vector<GameObject*> game_objects;
	GameObject* root = nullptr;
};

